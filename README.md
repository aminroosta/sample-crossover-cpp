# This project is also available on [gihub](https://github.com/aminroosta/Crossover-cpp)

There are 4 projects in the visual studio solution
* *Client* : WCF client side application (100% complete).
* *Service* : An static library that implements restfull api service(100% completle).
* *ServiceRunner* : A console application that runs the servece on 127.0.0.1:3000
* *TestsRunner* : A console application that runs the unit tests(100% complete).


Open the project and run *TestsRunner* for tests.
Run the *ServiceRunner* to start the webservice on `127.0.0.1:3000`
You can now run the *Client* app to interact with the webserver.
All webserver routes return *json*, you can use the browser to call apis as well.

*Service* uses cpprestsdk, i've implemented all operations via nonblocking `pplx::task<T>` methods. If you delete `/ServiceRunner/users.json` file the service will create a new list of dummy users. For unit tests see the *TestsRunner* porject.

# Screenshots of calling *Service* from browser:
![Alt text](screenshots/11.png?raw=true "Service Rest API")
![Alt text](screenshots/12.png?raw=true "Service Rest API")
![Alt text](screenshots/13.png?raw=true "Service Rest API")
![Alt text](screenshots/14.png?raw=true "Service Rest API")
![Alt text](screenshots/15.png?raw=true "Service Rest API")
![Alt text](screenshots/16.png?raw=true "Service Rest API")
![Alt text](screenshots/17.png?raw=true "Service Rest API")

# Screenshots of *Client* app:
![Alt text](screenshots/0.png?raw=true "WCF Client App")
![Alt text](screenshots/1.png?raw=true "WCF Client App")
![Alt text](screenshots/2.png?raw=true "WCF Client App")
![Alt text](screenshots/3.png?raw=true "WCF Client App")
![Alt text](screenshots/4.png?raw=true "WCF Client App")
![Alt text](screenshots/5.png?raw=true "WCF Client App")
![Alt text](screenshots/6.png?raw=true "WCF Client App")
![Alt text](screenshots/7.png?raw=true "WCF Client App")
![Alt text](screenshots/8.png?raw=true "WCF Client App")
![Alt text](screenshots/9.png?raw=true "WCF Client App")
![Alt text](screenshots/10.png?raw=true "WCF Client App")
