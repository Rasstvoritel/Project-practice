#include <iostream>

#include "library.h"
#include "soldiers.h"

static std::string ARR_CONDITIONS[4];
static std::string ARR_SPECIALIZATION[8];
static std::string ARR_RANG[6];

void library::Table::add_record(library::table_cell* info) {
    this->vec_of_table_cells.push_back(info);
    this->count_of_cells++;
}

void library::Table::add_record(Soldier& soldier) {
    auto new_table_cell =  new table_cell;
    new_table_cell->person = &soldier;
    new_table_cell->condition = healthy;
    new_table_cell->description = "I will add smt later";
    new_table_cell->idPVT = global_count_of_id_people++;
    add_record(new_table_cell);
}

std::ostream &library::Table::print(std::ostream &os) {
    auto tmp = this->vec_of_table_cells.begin();
    while (tmp!= this->vec_of_table_cells.end()){
        (*tmp)->person->print(os);
        os<<std::endl;
        tmp++;
    }
    return os;
}

void library::combat::add_record_to_table(std::istream &is) {

    std::string s;
    std::vector<std::string> words;
    std::getline(is, s);
    tokenize(s, ' ', words);

    int type;

    if(words.size() != 6) {return;}

    try {
        type = stoi(words[0]);
    } catch (std::invalid_argument &q) {
        return;
    }

    try {

        switch (type) {
            case 0:{  ///PVT
                Soldier* new_soldier = (Soldier*) new PVT(words[1], words[2],
                                      library::rang(stoi(words[3])), stoi(words[4]),
                                      library::specialization(stoi(words[5])));
                this->add_record_to_table(new_soldier);
                break;
            }
                ///StaffOfficer
            case 1:{
                Soldier* new_soldier =  (Soldier*) new StaffOfficer(words[1], words[2],
                                                              library::rang(stoi(words[3])), stoi(words[4]),
                                                              library::specialization(stoi(words[5])));
                this->add_record_to_table(new_soldier);
                break;
            }
                ///MainCommander
            case 2:{
                auto div = library::division();
                Soldier* new_soldier = (Soldier*) new MainCommander(words[1], words[2],
                                                          library::rang(stoi(words[3])), stoi(words[4]),
                                                          library::specialization(stoi(words[5])), div);
                this->add_record_to_table(new_soldier);
                break;
            }
                ///CombatCommander
            case 3:{

                auto div = library::division();
                Soldier* new_soldier = (Soldier*) new CombatCommander(words[1], words[2],
                                                        library::rang(stoi(words[3])), stoi(words[4]),
                                                        library::specialization(stoi(words[5])), div);
                this->add_record_to_table(new_soldier);
                break;
            }
            default:
                break;
        }

    }
    catch(std::invalid_argument& q) {
        return;
    }
}

void library::combat::add_record_to_table(library::Soldier* soldier) {
    auto new_table_cell =  new table_cell;
    new_table_cell->person = soldier;
    new_table_cell->condition = healthy;
    new_table_cell->description = "I will add smt later";
    new_table_cell->idPVT = global_count_of_id_people++;

    this->table.add_record(new_table_cell);

}

void library::combat::print(std::ostream& os) {
    this->table.print(os);
}

void library::shell::add_division(library::division *new_div) {
    this->list_of_divisions.push_back(new_div);
}

void library::shell::add_smb_to_division(library::shell::combatIterator iter, library::Soldier &smb) {

}

std::ostream &library::shell::print_ALL(std::ostream &os) {
    auto iter_start = this->begin();
    std::string who;

    while (iter_start != this->end()){
        auto tmp = *iter_start;
        size_t a = tmp.person->identification();
        switch (a) {
            case 0:{ who = "pvt\t"; break;}
            case 1:{ who = "staff officer\t";break;}
            case 2:{ who = "main commander\t";break;}
            case 3:{ who = "combat commander\t";break;}
        }

        std::string str = std::to_string(tmp.idPVT) + "\t" + who + tmp.person->get_name() + "\t" + tmp.person->get_surname() + "\t" +
        std::to_string(tmp.person->get_size()) + "\t" + ARR_RANG[tmp.person->get_rang()] + "\t" + ARR_SPECIALIZATION[tmp.person->get_specialization()] + "\t" +
        ARR_CONDITIONS[tmp.condition] + tmp.description;

        os<<str<<std::endl;
        ++iter_start;
    }
    return os;
}
