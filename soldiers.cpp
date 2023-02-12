#include "soldiers.h"
#include <cstdlib>
#include <iostream>
#include <iterator>

static std::string ARR_CONDITIONS[4] = {"healthy", "wounded", "killed", "missing"};
static std::string ARR_SPECIALIZATION[8] = {"infantry", "special_forces", "scout", "sniper",
                                            "sapper", "RKhPT", "medic", "radio_operator"};
static std::string ARR_RANG[6] = {"cadet", "lieutenant", "major", "brigadier", "general", "marshal"};

Queue<std::string> library::PVT::get_all_info() {

    Queue<std::string> list_of_all;

    auto queue_pointer = this->orders.begin();

    while (queue_pointer != this->orders.end()){
        auto tmp = *queue_pointer;
        std::string string = std::to_string(tmp->id_order) + "\t"
                          + std::to_string(tmp->time.day) + "."
                          + std::to_string(tmp->time.month) + "."
                          + std::to_string(tmp->time.year) + "\t"
                          + tmp->text;
        list_of_all.push(string);
    }

    return list_of_all;
}

std::ostream& library::PVT::print(std::ostream &os) const {
    os<<this->name<<" "<<this->surname<<" "
    <<ARR_RANG[this->rang]<<" "<<this->size<<" "
    <<ARR_SPECIALIZATION[this->specialization];
    return os;
}

void tokenize(std::string const &str, const char delim, std::vector<std::string> &out) {
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

std::istream &library::PVT::set_data(std::istream &is) {

    std::string s;
    std::vector<std::string> words;
    std::getline(is, s);
    tokenize(s, ' ', words);

    if(words.size() != 5) {
        return is;
    }

    try {
        this->size = stoi(words[3]);
        this->rang = library::rang(stoi(words[2]));
        this->specialization = library::specialization(stoi(words[4]));
    }
    catch(std::invalid_argument e) {
        return is;
    }

    this->name = words[0];
    this->surname = words[1];

        return is;

}
