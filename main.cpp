#include "crow.h"
#include "library.h"
#include "soldiers.h"

static std::string CONDITIONS[4] = {"healthy", "wounded", "killed", "missing"};
static std::string SPECIALIZATION[8] = {"infantry", "special_forces", "scout", "sniper",
                                        "sapper", "RKhPT", "medic", "radio_operator"};
static std::string RANG[6] = {"cadet", "lieutenant", "major", "brigadier", "general", "marshal"};
static std::string WHO[4] = {"PVT", "StaffOfficer", "MainCommander", "CombatCommander"};

int main()
{

    crow::SimpleApp app; //define your crow application

    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load("main_page.html");
        crow::mustache::context ctx ({{"person", "aaa"}});
        return page.render(ctx);

    });


    CROW_ROUTE(app, "/people")([]() {
        std::fstream in(R"(C:\Users\hrg\Desktop\people.txt)");
        std::fstream in2(R"(C:\Users\hrg\Desktop\people2.txt)");
        std::fstream in3(R"(C:\Users\hrg\Desktop\people3.txt)");
        auto div = library::division(in);
        auto div2 = library::division(in2);
        auto div3 = library::division(in3);

        std::vector<library::division> mass_of_divisions;
        mass_of_divisions.push_back(div);
        mass_of_divisions.push_back(div2);
        mass_of_divisions.push_back(div3);

        auto total = crow::json::wvalue::list();
        int count = 1;

        for (auto i: mass_of_divisions) {
            auto shell = library::shell(&i);
            auto it = shell.begin();

            auto division = crow::json::wvalue::object();
            division["id"] = count + 100;

            auto humans = crow::json::wvalue::list();

            while(it != shell.end()){
                auto human1 = crow::json::wvalue::object();
                human1["who"] = WHO[(*it).person->identification()];
                human1["name"] = (*it).person->get_name();
                human1["surname"] = (*it).person->get_surname();
                human1["size"] = std::to_string((*it).person->get_size());
                human1["rang"] = RANG[(*it).person->get_rang()];
                human1["specialization"] = SPECIALIZATION[(*it).person->get_specialization()];
                humans.emplace_back(human1);
                ++it;
            }
            count++;

            division["humans"] = crow::json::wvalue(humans);
            total.emplace_back(division);
        }

        auto page = crow::mustache::load("people.html");
        crow::mustache::context ctx ({{"total", total}});

        return page.render(ctx);
    });


    CROW_ROUTE(app, "/<int>")([](int num){
        return "It is " + std::to_string(num);

    });

    CROW_ROUTE(app, "/<string>")([](std::string name){
        return "Hello," + name;

    });

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
}