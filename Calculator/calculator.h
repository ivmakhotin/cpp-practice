#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

class Tokenizer {
public:
    Tokenizer(std::istream* in) : in_(in) {
    }

    enum TokenType { kUnknown, kNumber, kSymbol, kEnd };

    void Consume() {
        while (!in_->eof() && in_->peek() == ' ') {
            in_->get();
        }
        if (in_->eof()) {
            type_ = TokenType::kEnd;
            return;
        }
        if (isdigit(in_->peek())) {
            std::string number;
            while (isdigit(in_->peek())) {
                number += in_->get();
            }
            number_ = std::stod(number);
            type_ = TokenType::kNumber;
            return;
        }
        char c = in_->peek();
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            in_->get();
            symbol_ = c;
            type_ = TokenType::kSymbol;
            return;
        }
        type_ = TokenType::kUnknown;
    }

    TokenType GetType() {
        return type_;
    }

    int64_t GetNumber() {
        return number_;
    }

    char GetSymbol() {
        return symbol_;
    }

private:
    std::istream* in_;

    TokenType type_ = TokenType::kUnknown;
    int64_t number_;
    char symbol_;
};

class Expression {
public:
    virtual ~Expression() {
    }
    virtual int64_t Evaluate() = 0;
};

class Number : public Expression {
public:
    Number(int64_t number) : number_(number) {
    }
    virtual int64_t Evaluate() override {
        return number_;
    }

private:
    int64_t number_;
};

class UnaryNegation : public Expression {
public:
    UnaryNegation(std::unique_ptr<Expression> expression) : expression_(std::move(expression)) {
    }
    virtual int64_t Evaluate() final {
        return -expression_->Evaluate();
    }

private:
    std::unique_ptr<Expression> expression_;
};

class BinaryOperation : public Expression {
public:
    BinaryOperation(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

class Sum : public BinaryOperation {
public:
    explicit Sum(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }
    virtual int64_t Evaluate() final {
        return left_->Evaluate() + right_->Evaluate();
    }
};

class Minus : public BinaryOperation {
public:
    explicit Minus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }
    virtual int64_t Evaluate() final {
        return left_->Evaluate() - right_->Evaluate();
    }
};

class Mul : public BinaryOperation {
public:
    explicit Mul(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }
    virtual int64_t Evaluate() final {
        return left_->Evaluate() * right_->Evaluate();
    }
};

class Div : public BinaryOperation {
public:
    explicit Div(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : BinaryOperation(std::move(left), std::move(right)) {
    }
    virtual int64_t Evaluate() final {
        return left_->Evaluate() / right_->Evaluate();
    }
};

std::unique_ptr<Expression> ParseExpression(Tokenizer* tok);
std::unique_ptr<Expression> ParseSummand(Tokenizer* tok);
std::unique_ptr<Expression> ParseFactor(Tokenizer* tok);

std::unique_ptr<Expression> ParseExpression(Tokenizer* tok) {
    auto left = ParseSummand(tok);
    while (tok->GetType() == Tokenizer::TokenType::kSymbol &&
           (tok->GetSymbol() == '+' || tok->GetSymbol() == '-')) {
        if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '+') {
            auto right = ParseSummand(tok);
            left = std::make_unique<Sum>(std::move(left), std::move(right));
        } else if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '-') {
            auto right = ParseSummand(tok);
            left = std::make_unique<Minus>(std::move(left), std::move(right));
        } else {
            assert(false);
        }
    }
    return left;
}

std::unique_ptr<Expression> ParseSummand(Tokenizer* tok) {
    auto left = ParseFactor(tok);
    tok->Consume();
    while (tok->GetType() == Tokenizer::TokenType::kSymbol &&
           (tok->GetSymbol() == '*' || tok->GetSymbol() == '/')) {
        if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '*') {
            auto right = ParseFactor(tok);
            left = std::make_unique<Mul>(std::move(left), std::move(right));
        } else if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '/') {
            auto right = ParseFactor(tok);
            left = std::make_unique<Div>(std::move(left), std::move(right));
        } else {
            assert(false);
        }
        tok->Consume();
    }
    return left;
}

std::unique_ptr<Expression> ParseFactor(Tokenizer* tok) {
    tok->Consume();

    bool negative = false;
    if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '-') {
        negative = true;
        tok->Consume();
    } else if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '+') {
        negative = false;
        tok->Consume();
    }

    if (tok->GetType() == Tokenizer::TokenType::kNumber) {
        auto number = std::make_unique<Number>(tok->GetNumber());
        if (negative) {
            return std::make_unique<UnaryNegation>(std::move(number));
        } else {
            return number;
        }
        return number;
    } else if (tok->GetType() == Tokenizer::TokenType::kSymbol && tok->GetSymbol() == '(') {
        auto expression = ParseExpression(tok);
        assert(tok->GetType() == Tokenizer::TokenType::kSymbol);
        assert(tok->GetSymbol() == ')');
        if (negative) {
            return std::make_unique<UnaryNegation>(std::move(expression));
        } else {
            return expression;
        }
    } else {
        assert(false);
    }
}
