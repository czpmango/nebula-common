/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef COMMON_EXPRESSION_AGGREGATEEXPRESSION_H_
#define COMMON_EXPRESSION_AGGREGATEEXPRESSION_H_

#include "common/expression/Expression.h"
#include "common/function/AggregateFunction.h"
#include <common/datatypes/Set.h>

namespace nebula {

class AggregateExpression final : public Expression {
    friend class Expression;

public:
    AggregateExpression(std::string* name = nullptr,
                        Expression* arg = nullptr,
                        bool distinct = false)
        : Expression(Kind::kAggregate) {
        arg_.reset(arg);
        name_.reset(name);
        distinct_ = distinct;
    }


    const Value& eval(ExpressionContext& ctx) override;

    bool operator==(const Expression& rhs) const override;

    std::string toString() const override;

    void accept(ExprVisitor* visitor) override;

    std::unique_ptr<Expression> clone() const override {
        auto arg = arg_->clone();
        return std::make_unique<AggregateExpression>(new std::string(*name_),
                                                     std::move(arg).release(),
                                                     distinct_);
    }

    const std::string* name() const {
        return name_.get();
    }

    const Expression* arg() const {
        return arg_.get();
    }

    Expression* arg() {
        return arg_.get();
    }

    void setArg(Expression* arg) {
        arg_.reset(arg);
    }

    bool distinct() {
        return distinct_;
    }

    void setDistinct(bool dist) {
        distinct_ = dist;
    }

    const AggFun* aggFun() const {
        return fun_;
    }

    AggFun* aggFun() {
        return fun_;
    }

    void setAggFun(AggFun* agg_fun) {
        fun_ = agg_fun;
    }


private:
    void writeTo(Encoder& encoder) const override;

    void resetFrom(Decoder& decoder) override;

    std::unique_ptr<std::string>    name_;
    std::unique_ptr<Expression>     arg_;
    bool                            distinct_{false};
    AggFun*                        fun_{nullptr};
};

}  // namespace nebula
#endif  // EXPRESSION_AGGREGATEEXPRESSION_H_
