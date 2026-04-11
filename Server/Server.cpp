#include "AppConfig.hpp"
#include "CardHttpServer/CardHttpServer.hpp"

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "./TARO/TARO.hpp"
#include "TARO/TARO.hpp"
#include "LENORMAND/LENORMAND.hpp"

namespace net = boost::asio;

int main()
{
    try
    {
        const AppConfig config = LoadConfig("config.json");

        net::io_context ioContext;
        // io_context — это движок событий или диспетчер асинхронной работы.
        //    Он отвечает за такие вещи :
        //    ожидание входящих соединений
        //    чтение данных из сокетов
        //    запись данных в сокеты
        //    таймеры
        //    сигналы
        //    выполнение callback - ов
        //    выполнение coroutine - операций через co_await


        TARO taro;
        LENORMAND lenormand;

        taro.preloadAllCards();
        lenormand.preloadAllCards();

        auto server = std::make_shared<CardHttpServer>(
            ioContext,
            config.host,
            config.port,
            taro, 
            lenormand
        );

        server->Start();

        net::signal_set signals(ioContext, SIGINT, SIGTERM);
        //    SIGINT — прерывание, обычно Ctrl + C
        //    SIGTERM — запрос завершения процесса

		// signal_set это удобный способ обрабатывать сигналы в асинхронном коде. Когда процесс получает указанные сигналы, io_context будет уведомлен, и мы сможем выполнить нужные действия, например, корректно завершить сервер.
        signals.async_wait([&](const auto&, int)
            {
                ioContext.stop();
            });

        std::vector<std::jthread> workers;
        const std::size_t threadCount = std::max<std::size_t>(1, config.threads);

        for (std::size_t i = 0; i < threadCount; ++i)
        {
            workers.emplace_back([&ioContext]()
                {
                    ioContext.run();
                });
        }

        std::cout << "Server started on " << config.host << ":" << config.port << '\n';
        std::cout << "Threads: " << threadCount << '\n';

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}


//io_context
//Главный диспетчер событий.
//
//acceptor
//Слушает порт и принимает новых клиентов.
//
//socket
//Соединение с одним клиентом.
//
//flat_buffer
//Временное хранилище сырых входящих байтов.
//
//http::request
//Разобранный HTTP - запрос.
//
//http::response
//Ответ, который отправим клиенту.
//
//
//co_await
//Асинхронно подождать завершения операции.
//
//co_spawn
//Запустить корутину.
//
//signal_set
//Следить за Ctrl + C и корректно остановить сервер.