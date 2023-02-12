#ifndef DATABASE_LIBRARY_H
#define DATABASE_LIBRARY_H
#pragma once

#include <string>
#include <vector>
#include <set>
//#include "soldiers.h"
#include <memory>
#include <iostream>
#include "queue.h"
#include <sstream>
#include <fstream>

static size_t global_count_of_id_people = 0;
static size_t global_count_of_id_instruction = 0;
static size_t global_count_of_id_order = 0;
static size_t global_count_of_id_combat = 0;

void tokenize(std::string const &str, const char delim, std::vector<std::string> &out);

namespace library{
    class shell;

    enum conditions {healthy, wounded, killed, missing};
    enum specialization{infantry, special_forces, scout, sniper, sapper, RKhPT, medic, radio_operator};
    enum rang{cadet, lieutenant, major, brigadier, general, marshal};

    typedef struct time{
        size_t day;
        size_t month;
        size_t year;
    }time;

    typedef struct instructions{
        size_t id_instruction;
        std::string text;
    }instruction;

    typedef struct order{
        size_t id_order;
        std::string text;
        time time;

    }order;

    class Soldier;

    struct table_cell{
        size_t idPVT;
        std::string description;
        library::conditions condition;
        library::Soldier* person;
    };

    class Table{
        friend class shell;

    private:
        size_t count_of_cells;
        std::vector<table_cell*> vec_of_table_cells;

    public:

        Table(){
            this->count_of_cells = 0;
        }

//        copy
        Table(const Table& old) {
            for (auto &i: old.vec_of_table_cells) {
                this->add_record(i);
            }
            this->count_of_cells = this->vec_of_table_cells.size();
        }

        Table(Soldier& soldier){
            add_record(soldier);
            this->count_of_cells = vec_of_table_cells.size();
        }

        void add_record(table_cell* info);
        void add_record(Soldier& soldier);
        size_t size(){
            return this->vec_of_table_cells.size();
        }
        std::ostream& print(std::ostream&);

    };

    class CombatCommander;

    class training{
    private:
        const CombatCommander& com1;
        const CombatCommander& com2;
        int x_coordinate;
        int y_coordinate;
        time* time_start;
        size_t duration;
    public:
        void includePVT_to_training(size_t id);
        void kickPVT_from_training(size_t id);
    };


    class combat{
        friend class shell;
    private:
        size_t id_combat;
        Table table;
        int x_coordinate;
        int y_coordinate;
        std::string colour;
        char symbol;


    public:
        combat(){
            this->id_combat = global_count_of_id_combat++;
            this->table = Table();
            this->x_coordinate = 0;
            this->y_coordinate = 0;
            this->symbol = 'A';
        }


        combat(int x, int y){
            this->table = Table();
            this->id_combat = global_count_of_id_combat++;
            this->x_coordinate = x;
            this->y_coordinate = y;
            this->symbol = 'A';
        }

        combat(Table& table){
            this->table = table;
            this->id_combat = global_count_of_id_combat++;
            this->x_coordinate = 0;
            this->y_coordinate = 0;
            this->symbol = 'A';
        }

        combat(std::fstream& in){
            this->table = Table();
            if (in.is_open()){
                std::string line;
                while (getline(in, line)){
                    std::istringstream is(line);
                    this->add_record_to_table(is);
                    line="";
                }
            }
            in.close();
            this->id_combat = global_count_of_id_combat++;
            this->x_coordinate = 0;
            this->y_coordinate = 0;
            this->symbol = 'A';

        }


        void add_record_to_table(std::istream &is);
        void add_record_to_table(Soldier* soldier);
        void print(std::ostream& os);
        size_t size(){
            return this->table.size();
        }
        const Table& show_table() const{
            return this->table;
        }
    };

    class division: public combat{

    public:
        division():combat(){};
        division(int x, int y): combat(x, y){};
        division(Table& table):combat(table){};
        division(std::fstream& in):combat(in){};
    };


    class shell{
    private:
        std::vector<division*> list_of_divisions;

        class combatIterator{

            friend class division;
            friend class shell;

        private:
            std::vector<division*>::iterator current;
            std::vector<division*>::iterator end;
            std::vector<table_cell*>::iterator cell_cur;

            combatIterator(std::vector<division*>::iterator start, std::vector<division*>::iterator end){
                auto tmp = *start;
                this->current = start;
                this->end = end;
                if(start!=end){
                    this->cell_cur = (*current)->table.vec_of_table_cells.begin();
                }
            }

        public:
            const table_cell& operator*(){
                    return *(*(this->cell_cur));
            }

            combatIterator& operator++(){
                auto aaa = (*this->current)->table.vec_of_table_cells.end();
                aaa--;
                if ((this->cell_cur) != aaa){
                    this->cell_cur++;
                    return *this;
                }else{
                    auto& bbb = current;
                    bbb++;
                    if (bbb != this->end){
                        this->cell_cur = (*bbb)->table.vec_of_table_cells.begin();
                        return *this;
                    }else{
                        return *this;
                    }
                }

            };

            combatIterator& operator++(int){
                auto aaa = (*this->current)->table.vec_of_table_cells.end();
                aaa--;
                auto tmp = this;
                if((*this->cell_cur) != *aaa){
                    cell_cur++;
                    return *tmp;
                } else{
                    if ((this->current++) != this->end){
                        this->cell_cur = (*current)->table.vec_of_table_cells.begin();
                        return *tmp;
                    } else{
                        return *this;
                    }
                }
            };

            bool operator!=(combatIterator iter2){
                if (this->current == iter2.current){
                    return false;
                } else{
                    return true;
                }
            }

            bool operator==(combatIterator iter2){
                if (this->current == iter2.current){
                    return true;
                } else{
                    return false;
                }
            }
        };

    public:

        shell()= default;

        shell(division* division){
            list_of_divisions.push_back(division);
        }

        std::ostream& print_ALL(std::ostream& os);
        void add_division(division* new_div);
        void add_smb_to_division(combatIterator iter, Soldier& smb);

        combatIterator begin(){
            return combatIterator(this->list_of_divisions.begin(), this->list_of_divisions.end());
        }

        combatIterator end(){
            auto it = this->list_of_divisions.end();
            return combatIterator(it, it);
        }
    };
}

#endif //DATABASE_LIBRARY_H
