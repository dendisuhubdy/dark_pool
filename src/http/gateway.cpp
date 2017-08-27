#include "node/loop.hpp"
#include "node/tcp.hpp"
#include "node/http.hpp"

#include <vector>
#include <iostream>

class WebRouter {
	public:
		WebRouter() {}
		
		void serve(market::node::http::request& req, market::node::http::response& res) {
			std::cout << req.get_header("User-Agent") << std::endl;
			std::cout << req.get_header("Accept") << std::endl;
			if ( req.url().path() == "/post" ) {
				res.set_status(200);
				res.set_header("Content-Type", "text/html");
				res.end("Post request");
			} else { 
				res.set_status(200);
				res.set_header("Content-Type", "text/html");
				res.end("Hey!");
			}
			
		}
};

int main(int argc, char *argv[]) {
    market::node::http::http server;
	WebRouter router;
    if(!server.listen("127.0.0.1", 3000, [&](market::node::http::request& req, market::node::http::response& res) {
		router.serve(req, res); 
    })) {
        std::cerr << "Failed to start server." << std::endl;
        return 1;
    }
    return market::node::run();
}
