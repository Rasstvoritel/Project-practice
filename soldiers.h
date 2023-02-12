#ifndef DATABASE_SOLDIERS_H
#define DATABASE_SOLDIERS_H
#pragma once

#include <utility>

#include "queue.h"
#include "library.h"

//enum conditions {healthy, wounded, killed, missing};
//enum specialization{infantry, special_forces, scout, sniper, sapper, RKhPT, medic, radio_operator};
//enum rang{cadet, lieutenant, major, brigadier, general, marshal};

namespace library {
    //! virtual basic class
//!soldier class
    class Soldier {
    protected:
        std::string name;
        std::string surname;
        library::rang rang;
        size_t size;
        library::specialization specialization;

    public:
        virtual ~Soldier() = default;

        virtual std::ostream &print(std::ostream &os) const = 0;

        virtual std::istream &set_data(std::istream &) = 0;  //overload to all fields

        const std::string &get_name() const { return this->name; };

        const std::string &get_surname() const { return this->surname; };

        const library::specialization get_specialization() const { return this->specialization; };

        const library::rang get_rang() const { return this->rang; };

        const size_t get_size() const { return this->size; };

//        - собрать все инструкции и приказы и вывести в виде списка строк
        virtual Queue<std::string> get_all_info() = 0;

        virtual size_t identification() const = 0;

    };

    class PVT : public Soldier {
    protected:
        size_t identifier;
        void set_identifier(size_t id){
            this->identifier = id;
        }

    private:
        library::conditions condition = healthy; //basic
        Queue<library::order *> orders;
    public:

        PVT(std::string name, std::string surname, library::rang rang, size_t size,
            library::specialization specialization): Soldier(), identifier(0){
            this->name = std::move(name);
            this->surname = std::move(surname);
            this->rang = rang;
            this->size = size;
            this->specialization = specialization;
        }

        ~PVT() override {
            while (!this->orders.empty()) {
                this->orders.pop();
            }
        }

        const library::conditions get_condition() const { return condition; };

        Queue<std::string> get_all_info() override;

        std::ostream &print(std::ostream &os) const override;

        std::istream &set_data(std::istream &is) override;

        size_t identification() const override{
            return this->identifier;
        }


    };

    class StaffOfficer : public PVT {
    private:
        Queue<library::instruction *> instructions;
    public:
        StaffOfficer(std::string name, std::string surname, library::rang rang, size_t size,
                     library::specialization specialization) :
                PVT(std::move(name), std::move(surname), rang, size, specialization) {
            PVT::set_identifier(1);};
    };


    class MainCommander : public StaffOfficer {
    protected:
        library::division &division;
    public:

        MainCommander(std::string name, std::string surname, library::rang rang, size_t size,
                      library::specialization specialization, library::division &division) :
                StaffOfficer(std::move(name), std::move(surname), rang, size, specialization), division(division) {
            PVT::set_identifier(2);
        };
    };

//!cobatcommander class
    class CombatCommander : public MainCommander {

    public:
        CombatCommander(std::string name, std::string surname, library::rang rang, size_t size,
                library::specialization specialization, library::division &division) :
        MainCommander(std::move(name), std::move(surname), rang, size, specialization, division) {
            PVT::set_identifier(3);
        };
    };
}
#endif //DATABASE_SOLDIERS_H
