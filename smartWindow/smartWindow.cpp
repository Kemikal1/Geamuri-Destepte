#include <algorithm>
#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>

#include <signal.h>

using namespace std;
using namespace Pistache;

// General advice: pay atetntion to the namespaces that you use in various contexts. Could prevent headaches.

// This is just a helper function to preety-print the Cookies that one of the enpoints shall receive.
void printCookies(const Http::Request& req) {
    auto cookies = req.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto& c: cookies) {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
}

// Some generic namespace, with a simple function we could use to test the creation of the endpoints.
namespace Generic {

    void handleReady(const Rest::Request&, Http::ResponseWriter response) {
        
        response.send(Http::Code::Ok, "Your window is ready to set the properties! \n");
    }

}

// Definition of the SmartWindowsEndPoints class 
class SmartWindowEndpoint {
public:
    explicit SmartWindowEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    // Initialization of the server. Additional options can be provided here
    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        // Server routes are loaded up
        setupRoutes();
    }

    // Server is started threaded.  
    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serveThreaded();
    }

    // When signaled server shuts down
    void stop(){
        httpEndpoint->shutdown();
    }

private:
    void setupRoutes() {
        using namespace Rest;
        // Defining various endpoints
        // Generally say that when http://localhost:9080/ready is called, the handleReady function should be called. 
        // For the default level of heliomare 
        Routes::Get(router, "/ready", Routes::bind(&Generic::handleReady));
        // Routes::Get(router, "/auth", Routes::bind(&MicrowaveEndpoint::doAuth, this));
        // update the level of heliomare coresponding to the value in the route
        // update the incalzire property coresponding to the value in the route
        Routes::Post(router, "/properties/:propertyName/:value", Routes::bind(&SmartWindowEndpoint::setProperty, this));
        // for this Route will receive the default level of heliomare: 0, if the `heliomare` was not set before
        // for this Route will receive the default value for incalzire: Off, if the `incalzire` was not set before
        Routes::Get(router, "/properties/:propertyName/", Routes::bind(&SmartWindowEndpoint::getProperty, this));

    }

    
    // void doAuth(const Rest::Request& request, Http::ResponseWriter response) {
    //     // Function that prints cookies
    //     printCookies(request);
    //     // In the response object, it adds a cookie regarding the communications language.
    //     response.cookies()
    //         .add(Http::Cookie("lang", "en-US"));
    //     // Send the response
    //     response.send(Http::Code::Ok);
    // }

    // Endpoint to configure one of the SmartWindow's properties.
    void setProperty(const Rest::Request& request, Http::ResponseWriter response){
        // You don't know what the parameter content that you receive is, but you should
        // try to cast it to some data structure. Here, I cast the propertyName to string.
        auto propertyName = request.param(":propertyName").as<std::string>();

        // This is a guard that prevents editing the same value by two concurent threads. 
        Guard guard(smartWindowLock);

        
        int val;
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            val = value.as<int>();
        }

        // Setting the smartWindow's property coresponding to value 
        // 1 2 3 (0 - error)
        // setResponse - the level of heliomare 
        int setResponse = sw.set(propertyName, val);

        // Sending some confirmation or error response.
        if(propertyName == "heliomare") {
            if (setResponse != -1) {
            response.send(Http::Code::Ok, propertyName + " was set to " + to_string(setResponse) + "\n");
            }
            else {
                response.send(Http::Code::Not_Found, to_string(val) + "' was not a valid value " + "\n");
            }   
        }
        else if ( propertyName == "incalzire") {
            if (setResponse != -1) {
                string resp = "Off";

                if(setResponse == 1) {
                    resp = "On";
                } 

                response.send(Http::Code::Ok, propertyName + " was set to " + resp + "\n");
            }
            else {
                response.send(Http::Code::Not_Found, to_string(val) + "' was not a valid value " + "\n");
            } 

        }
        else {
            
            response.send(Http::Code::Not_Found, propertyName + " was not found \n ");
        }



    }

    // Setting to get the settings value of one of the configurations of the Smartwindow
    void getProperty(const Rest::Request& request, Http::ResponseWriter response){
        auto propertyName = request.param(":propertyName").as<std::string>();

        Guard guard(smartWindowLock);

        string valueSetting = sw.get(propertyName);

        if (valueSetting != "") {

            // In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
            using namespace Http;
            response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));

            response.send(Http::Code::Ok, propertyName + " is " + valueSetting + "\n");
        }
        else {
            response.send(Http::Code::Not_Found, propertyName + " was not found" + "\n");
        }
    }

    // Defining the class of the smartWindow. It should model the entire configuration of the smartWindow
    class smartWindow {
    public:
        explicit smartWindow(){ }

        // Setting the value for one of the settings. Hardcoded for the heliomare option
        int set(std::string name, int value){
            if(name == "heliomare"){
                heliomare.name = name;
                if(value >= 1 && value <= 30){
                    heliomare.value = 1;
                    return 1;
                }
                else if(value > 30 && value <= 60){
                    heliomare.value = 2;
                    return 2;
                }
                else if (value > 60 && value <= 100) {
                    heliomare.value = 3;
                    return 3;
                }
                return -1;
            }
            else if(name == "incalzire") {
                incalzire.name = name;
                if(value <= 0){
                    incalzire.value = "On";
                    return 1;
                }
                else if(value > 0) 
                {
                    incalzire.value = "Off";
                    return 0;
                }
                return -1;
            }
            return -1;
        }

        // Getter
        string get(std::string name){
            if (name == "heliomare"){
                return std::to_string(heliomare.value);
            }
            else if(name == "incalzire") {
                return incalzire.value;
            }
            else{
                return "";
            }
        }

    private:
        // Defining and instantiating properties.
        struct heliProperty{
            std::string name;
            int value = 0;
        }heliomare;

        struct incalzireProperty{
            std::string name;
            std::string value = "Off";
        }incalzire;
    };

    // Create the lock which prevents concurrent editing of the same variable
    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock smartWindowLock;

    // Instance of the smartWindow model
    smartWindow sw;

    // Defining the httpEndpoint and a router.
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {

    // This code is needed for gracefull shutdown of the server when no longer needed.
    sigset_t signals;
    if (sigemptyset(&signals) != 0
            || sigaddset(&signals, SIGTERM) != 0
            || sigaddset(&signals, SIGINT) != 0
            || sigaddset(&signals, SIGHUP) != 0
            || pthread_sigmask(SIG_BLOCK, &signals, nullptr) != 0) {
        perror("install signal handler failed");
        return 1;
    }

    // Set a port on which your server to communicate
    Port port(9080);

    // Number of threads used by the server
    int thr = 2;

    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thr = std::stoi(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    // Instance of the class that defines what the server can do.
    SmartWindowEndpoint stats(addr);

    // Initialize and start the server
    stats.init(thr);
    stats.start();


    // Code that waits for the shutdown sinal for the server
    int signal = 0;
    int status = sigwait(&signals, &signal);
    if (status == 0)
    {
        std::cout << "received signal " << signal << std::endl;
    }
    else
    {
        std::cerr << "sigwait returns " << status << std::endl;
    }

    stats.stop();
}