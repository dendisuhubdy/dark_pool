#include "model/order_book.hpp"
#include "model/order.hpp"

#include "disruptor/ring_buffer.hpp"
#include "disruptor/event_publisher.hpp"
#include "disruptor/exception_handler.hpp"
#include "disruptor/event_processor.hpp"

#include <sys/time.h>
#include <iostream>
#include <array>
#include <thread>
#include <future>
#include <functional>

// The OrderBufferFactory is responsible for initializing the backing store for the RingBuffer.
class OrderBufferFactory : public market::disruptor::EventFactoryInterface<market::model::Order>
{ 
    public:
        market::model::Order*
        NewInstance(
            const int& size
        ) const
        {
            auto array = new market::model::Order[size];
            return array;
        }
};

// The OrderTranslator is responsible for marshaling input data into Order objects.
// This implementation creates a random order for testing purposes. 
class OrderTranslator : public market::disruptor::EventTranslatorInterface<market::model::Order>
{
    market::model::Order*
    TranslateTo(
        const int64_t& sequence,
        market::model::Order* order
    )
    {
        bool is_buy(rand() % 2);
        uint32_t delta = is_buy ? 1880 : 1884;
        market::book::Price price = (rand() % 10) + delta;
        market::book::Quantity qty = ((rand() % 10) + 1) * 100;
        order->initialize(is_buy, price, qty);
        return order;
    }
};

class OrderBatchHandler : public market::disruptor::EventHandlerInterface<market::model::Order>
{
    public:
        

        void OnEvent(
            const int64_t& sequence,
            const bool& end_of_batch,
            market::model::Order* order
        )
        {
            //order_book.add_order(order);
            //if (event) { 
            //    event->set_value(sequence);
            //}
        };

        void OnStart() {}
        void OnShutdown() {}
};

int main(int argc, char *argv[]) {
    int buffer_size = 1024 * 16;
    long iterations = 1000L * 1000L * 300;

    auto input_buffer = new market::disruptor::RingBuffer<market::model::Order>(
        new OrderBufferFactory,
        buffer_size,
        market::disruptor::kSingleThreadedStrategy, // A single publisher thread
        market::disruptor::kBusySpinStrategy
    );

    std::vector<market::disruptor::Sequence*> sequence_to_track(0);
    std::unique_ptr<market::disruptor::ProcessingSequenceBarrier> barrier(input_buffer->NewBarrier(sequence_to_track));

    OrderBatchHandler order_handler;
    market::disruptor::IgnoreExceptionHandler<market::model::Order> order_exception_handler;
    market::model::OrderBook processor(
        input_buffer,
        (market::disruptor::SequenceBarrierInterface*) barrier.get(),
        &order_handler,
        &order_exception_handler
    );

    std::thread consumer(std::ref<market::model::OrderBook>(processor));

    // TODO: Handle CPU affinity for the consumer thread.
    #ifdef __APPLE__
        std::cout << "Running on MacOS X" << std::endl;    
    #elif __FreeBSD__
        std::cout << "Running on FreeBSD" << std::endl;
    #endif 

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    std::unique_ptr<OrderTranslator> translator(new OrderTranslator);
    market::disruptor::EventPublisher<market::model::Order> publisher(input_buffer);
    for (long i=0; i<iterations; i++) {
        publisher.PublishEvent(translator.get());
    }

    long expected_sequence = input_buffer->GetCursor();
    while (processor.GetSequence()->sequence() < expected_sequence) {}

    gettimeofday(&end_time, NULL);

    double start, end;
    start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(15);
    std::cout << "Performance: ";
    std::cout << (iterations * 1.0) / (end - start) << " OPS" << std::endl;

    processor.Halt();
    consumer.join();
    
    return 0;
}
