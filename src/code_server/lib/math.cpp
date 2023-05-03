

#include "lib/math.hpp"

std::shared_ptr<discode::Data> lib_math::Sum::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto sum = 0;
    auto arr = data.at(0)->getVector();
    for (size_t i = 0; i < arr->size(); i++) { sum = sum + arr->at(i)->getNumber(); }
    return std::make_shared<discode::Number>(sum);
}

std::shared_ptr<discode::Data> lib_math::Mean::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto arr = data.at(0)->getVector();
    auto sum = 0;
    auto size = arr->size();
    for (size_t i = 0; i < arr->size(); i++) { sum = sum + arr->at(i)->getNumber(); }
    return std::make_shared<discode::Number>(sum / size);
}

std::shared_ptr<discode::Data> lib_math::Max::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto arr = data.at(0)->getVector();
    auto max = arr->at(0)->getNumber();
    for (size_t i = 0; i < arr->size(); i++) {
        if (arr->at(i)->getNumber() > max) { max = arr->at(i)->getNumber(); }
    }
    return std::make_shared<discode::Number>(max);
}

std::shared_ptr<discode::Data> lib_math::Min::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto arr = data.at(0)->getVector();
    auto min = arr->at(0)->getNumber();
    for (size_t i = 0; i < arr->size(); i++) {
        if (arr->at(i)->getNumber() < min) { min = arr->at(i)->getNumber(); }
    }
    return std::make_shared<discode::Number>(min);
}

std::shared_ptr<discode::Data> lib_math::Random::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto min = data.at(0)->getNumber();
    auto max = data.at(1)->getNumber();
    return std::make_shared<discode::Number>(rand() % static_cast<int>(max - min) + min);
}

std::shared_ptr<discode::Data> lib_math::Power::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto base = data.at(0)->getNumber();
    auto exp = data.at(1)->getNumber();
    auto result = base;
    for ( auto i = 1; i < exp; i++ ) { result = result * base; }
    return std::make_shared<discode::Number>(result);
}

std::shared_ptr<discode::Data> lib_math::Sqrt::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto num = data.at(0)->getNumber();
    double x = num;
    double y = 1;
    double e = 0.000001;
    while (x - y > e) {
        x = (x + y) / 2;
        y = num / x;
    }
    return std::make_shared<discode::Number>(num);
}

std::shared_ptr<discode::Data> lib_math::Mod::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto dividend = data.at(0)->getNumber();
    auto divisor = data.at(1)->getNumber();
    if (divisor == 0) { return std::make_shared<discode::Null>(); }
    int quotient = dividend / divisor;
    auto remainder = dividend - (quotient * divisor);
    if (dividend >= 0 || remainder == 0) { return std::make_shared<discode::Number>(remainder); }
    return std::make_shared<discode::Number>(remainder + divisor);
}

