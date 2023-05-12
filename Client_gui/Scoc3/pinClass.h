#ifndef PINCLASS_H
#define PINCLASS_H

#include <string>
#include <vector>

class pinClass {
   public:
    std::string module = NULL;
    std::string reg = NULL;
    std::string field = NULL;
    int type = 0;

    void initialize(std::string raw){
        for (int i = 2; i < raw.size(); i++) {
            if (raw.at(i)=='.') {
                this->type++;
            }
            else {
                switch (this->type) {
                case 0:
                    this->module.push_back(raw.at(i));
                    break;

                case 1:
                    this->reg.push_back(raw.at(i));
                    break;

                case 2:
                    this->field.push_back(raw.at(i));
                    break;
                }
            }
        }
    }
};

#endif // PINCLASS_H
