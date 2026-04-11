#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <optional>
#include <regex>
#include <string>

#include "../CardRepository/CardRepository.hpp"
#include "../TARO/TARO.hpp"
#include "../LENORMAND/LENORMAND.hpp"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = net::ip::tcp;

//Boost.Asio — это библиотека для :
//асинхронного ввода - вывода
//сетевого программирования
//сокетов
//таймеров
//сигналов
//coroutine - based async programming



// Дополнение по io_context — это центральный объект, который:
//хранит очередь работы
//управляет асинхронными операциями
//передает выполнение обработчикам
//является “сердцем” Asio - программы

class CardHttpServer : public std::enable_shared_from_this<CardHttpServer>
{
public:
    CardHttpServer(
        net::io_context& ioContext,
        std::shared_ptr<CardRepository> repository,
        std::string host,
        std::uint16_t port,
		TARO& taro,
        LENORMAND& lenormand
    )
        : ioContext_(ioContext)
        , acceptor_(net::make_strand(ioContext))
        , repository_(std::move(repository))
        , host_(std::move(host))
        , port_(port)
		, taro_(taro)
		, lenormand_(lenormand)
    {
    }

    void Start()
    {
        beast::error_code ec;
        auto address = net::ip::make_address(host_, ec);
        if (ec)
        {
            throw std::runtime_error("Invalid host address: " + host_);
        }

        tcp::endpoint endpoint{ address, port_ };

        acceptor_.open(endpoint.protocol(), ec);
        if (ec) throw beast::system_error(ec);

        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec) throw beast::system_error(ec);

        acceptor_.bind(endpoint, ec);
        if (ec) throw beast::system_error(ec);

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) throw beast::system_error(ec);

		net::co_spawn(  // co_spawn — это функция, которая запускает корутину на указанном экзекуторе (типа thread). Она принимает три аргумента:
            acceptor_.get_executor(),
            [self = shared_from_this()]() -> net::awaitable<void>
            {
				co_await self->AcceptLoop();    // Это корутина, которая будет бесконечно принимать входящие соединения и запускать для каждого из них обработчик сессии.
            },
            net::detached);
    }

private:
    using Request = http::request<http::string_body>;
    using Response = http::response<http::string_body>;

    enum class RouteType
    {
        None,
        Cards,
        Combination
    };

    enum class CardType
    {
        Taro,
        Lenormand
    };

    struct RouteInfo
    {
        RouteType routeType{ RouteType::None };
        CardType cardType{ CardType::Taro };
        int id{ 0 };
    };

	net::awaitable<void> AcceptLoop() // Этот метод будет бесконечно принимать входящие соединения и запускать для каждого из них обработчик сессии.
    {
    //    Что происходит 
    //    сервер ждет нового клиента
    //        как только клиент подключился, создается новый socket
    //        для этого клиента запускается отдельная coroutine HandleSession
    //        цикл сразу снова идет ждать следующего клиента
        for (;;)
        {
            tcp::socket socket = co_await acceptor_.async_accept(net::use_awaitable); // Это уже современный C++ coroutines.

            net::co_spawn(
				acceptor_.get_executor(),  // это экзекутор, который будет использоваться для выполнения обработчика. 
                // В данном случае мы используем тот же экзекутор, что и у acceptor_, чтобы обеспечить последовательное выполнение операций.
                [self = shared_from_this(), socket = std::move(socket)]() mutable -> net::awaitable<void>
                {
                    co_await self->HandleSession(std::move(socket));
                },
                net::detached);
        }
    }

    net::awaitable<void> HandleSession(tcp::socket socket)
    {
        //Для одного клиента :

        //создается flat_buffer
        //    в цикле читаются HTTP - запросы
        //    каждый запрос передается в HandleRequest
        //    формируется ответ
        //    ответ отправляется клиенту
        //    если keep - alive выключен — соединение закрывается

		beast::flat_buffer buffer;  // Этот буфер используется для хранения данных, читаемых из сокета. Он оптимизирован для работы с Boost.Beast и позволяет эффективно обрабатывать HTTP-сообщения.

        try
        {
            for (;;)
            {
                Request request;
                co_await http::async_read(socket, buffer, request, net::use_awaitable);

                Response response = HandleRequest(request);
                response.keep_alive(request.keep_alive());

                co_await http::async_write(socket, response, net::use_awaitable);

                if (!response.keep_alive())
                    break;
            }
        }
        catch (const beast::system_error& e)
        {
            if (e.code() != http::error::end_of_stream)
            {
                std::cerr << "HTTP session error: " << e.what() << '\n';
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "HTTP session exception: " << e.what() << '\n';
        }

        beast::error_code ec;
        socket.shutdown(tcp::socket::shutdown_send, ec);
        co_return;
    }

    Response HandleRequest(const Request& request)
    {
        //Эта функция :

        //смотрит путь, например / cards / 1
        //    понимает endpoint
        //    смотрит HTTP - метод
        //    вызывает нужную функцию :
        //HandleGet
        //    HandlePost
        //    HandleDelete
        try
        {
            const std::string path = std::string(request.target());

            if (request.method() == http::verb::options)
            {
                return MakeEmptyResponse(http::status::ok, request.version(), request.keep_alive());
            }

            const auto routeInfo = ParseRoute(path);
            if (!routeInfo.has_value())
            {
                return MakeJsonResponse(
                    http::status::not_found,
                    {
                        {"error", "Endpoint not found"}
                    },
                    request.version(),
                    request.keep_alive());
            }

            switch (request.method())
            {
            case http::verb::get:
                if (routeInfo->routeType == RouteType::Cards)
                    return HandleGet(routeInfo->id, request, routeInfo->cardType);
                break;

            case http::verb::post:
                if (routeInfo->routeType == RouteType::Combination)
                    return HandlePost(routeInfo->id, request, routeInfo->cardType);
                break;

            case http::verb::delete_:
                if (routeInfo->routeType == RouteType::Combination)
                    return HandleDelete(routeInfo->id, request, routeInfo->cardType);
                break;

            default:
                break;
            }

            return MakeJsonResponse(
                http::status::method_not_allowed,
                {
                    {"error", "Method not allowed for this endpoint"}
                },
                request.version(),
                request.keep_alive());
        }
        catch (const nlohmann::json::exception& e)
        {
            return MakeJsonResponse(
                http::status::bad_request,
                {
                    {"error", "Invalid JSON"},
                    {"details", e.what()}
                },
                request.version(),
                request.keep_alive());
        }
        catch (const std::exception& e)
        {
            return MakeJsonResponse(
                http::status::internal_server_error,
                {
                    {"error", "Internal server error"},
                    {"details", e.what()}
                },
                request.version(),
                request.keep_alive());
        }
    }

    Response HandleGet(const int id, const Request& request, CardType cardType)
    {
        nlohmann::json card;
        if (cardType == CardType::Taro)
        {
            card = taro_.getCardInfo(id);
        }
        else
        {
            card = lenormand_.getCardInfo(id);
        }
        //auto card = repository_->GetById(id);
		//if (!card.get.has_value())
  //      {
  //          return MakeJsonResponse(
  //              http::status::not_found,
  //              {
  //                  {"error", "Card not found"},
  //                  {"id", id}
  //              },
  //              request.version(),
  //              request.keep_alive());
  //      }
		std::cout << "Card data for ID " << id << ": " << card.dump(4) << std::endl;
        return MakeJsonResponse(
            http::status::ok,
            {
                {"data", card}
            },
            request.version(),
            request.keep_alive());
    }

    Response HandlePost(const int id, const Request& request, CardType cardType)
    {
        if (request.body().empty())
        {
            return MakeJsonResponse(
                http::status::bad_request,
                {
                    {"error", "Request body is empty"}
                },
                request.version(),
                request.keep_alive());
        }

        //auto j = nlohmann::json::parse(request.body());

        //Card card = j.get<Card>();
        //card.id = id;

        nlohmann::json card;
        if (cardType == CardType::Taro)
        {
            card = taro_.addCombination(id);
        }
        else
        {
            card = lenormand_.addCombination(id);
        }

        if (card.empty())
        {
            return MakeJsonResponse(
                http::status::bad_request,
                {
                    {"message", "Комбинация добавлена, но количество карт меньше 2."},
                    {"data", {}}
                },
                request.version(),
                request.keep_alive());
        }

        return MakeJsonResponse(
            http::status::created,
            {
                {"message", "Комбинация добавлена."},
                {"data", card}
            },
            request.version(),
            request.keep_alive());
    }

    Response HandleDelete(const int id, const Request& request, CardType cardType)
    {
        nlohmann::json card;
        if (cardType == CardType::Taro)
        {
            card = taro_.deleteCombination(id);
        }
        else
        {
            card = lenormand_.deleteCombination(id);
        }

        if (card.empty())
        {
            return MakeJsonResponse(
                http::status::not_found,
                {
                    {"error", "Карта была удалена из комбинации или отсутсвовала."},
                    {"data", card}
                },
                request.version(),
                request.keep_alive());
        }

        return MakeJsonResponse(
            http::status::ok,
            {
                {"message", "Карта удалена из комбинации или отсутствует."},
                {"data", card}
            },
            request.version(),
            request.keep_alive());
    }

    static std::optional<RouteInfo> ParseRoute(const std::string& path)
    {
        static const std::regex cardsPattern(R"(^/taro/cards/([0-9]+)$)");
        static const std::regex combinationPattern(R"(^/taro/combination/([0-9]+)$)");
        static const std::regex lenormandСardsPattern(R"(^/lenormand/cards/([0-9]+)$)");
        static const std::regex lenormandСombinationPattern(R"(^/lenormand/combination/([0-9]+)$)");

        std::smatch match;

        if (std::regex_match(path, match, cardsPattern))
        {
            try
            {
                return RouteInfo{ RouteType::Cards, CardType::Taro, std::stoi(match[1].str()) };
            }
            catch (...)
            {
                return std::nullopt;
            }
        }

        if (std::regex_match(path, match, lenormandСardsPattern))
        {
            try
            {
                return RouteInfo{ RouteType::Cards, CardType::Lenormand, std::stoi(match[1].str()) };
            }
            catch (...)
            {
                return std::nullopt;
            }
        }

        if (std::regex_match(path, match, combinationPattern))
        {
            try
            {
                return RouteInfo{ RouteType::Combination, CardType::Taro, std::stoi(match[1].str()) };
            }
            catch (...)
            {
                return std::nullopt;
            }
        }

        if (std::regex_match(path, match, lenormandСombinationPattern))
        {
            try
            {
                return RouteInfo{ RouteType::Combination, CardType::Lenormand, std::stoi(match[1].str()) };
            }
            catch (...)
            {
                return std::nullopt;
            }
        }

        return std::nullopt;
    }

    static void AddCorsHeaders(Response& response)
    {
        response.set(http::field::access_control_allow_origin, "*");
        response.set(http::field::access_control_allow_methods, "GET, POST, DELETE, OPTIONS");
        response.set(http::field::access_control_allow_headers, "Content-Type");
    }

    static Response MakeEmptyResponse(http::status status, unsigned version, bool keepAlive)
    {
        Response response{ status, version };
        response.set(http::field::server, "CardHttpServer");
        AddCorsHeaders(response);
        response.keep_alive(keepAlive);
        response.prepare_payload();
        return response;
    }

    static Response MakeJsonResponse(
        http::status status,
        const nlohmann::json& body,
        unsigned version,
        bool keepAlive)
    {
        Response response{ status, version };
        response.set(http::field::server, "CardHttpServer");
        response.set(http::field::content_type, "application/json; charset=utf-8");
        AddCorsHeaders(response);
        response.keep_alive(keepAlive);
        response.body() = body.dump(4);
        response.prepare_payload();
        return response;
    }

private:
    net::io_context& ioContext_;
    tcp::acceptor acceptor_;  // это объект, который слушает адрес и порт и принимает входящие TCP-соединения.
    std::shared_ptr<CardRepository> repository_;
    std::string host_;
    std::uint16_t port_;
    TARO taro_;
    LENORMAND lenormand_;
};

// curl -X POST http://127.0.0.1:8080/combination/1 -H "Content-Type: application/json" -d "{\"name\":\"The Fool\"}"
//
// curl -X DELETE http://127.0.0.1:8080/combination/1
//
// curl http://127.0.0.1:8080/cards/1