/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */
#include "common/expression/AggregateExpression.h"
#include "common/expression/ExprVisitor.h"

namespace nebula {
bool AggregateExpression::operator==(const Expression& rhs) const {
    if (kind_ != rhs.kind()) {
        return false;
    }

    const auto& r = static_cast<const AggregateExpression&>(rhs);
    return *name_ == *(r.name_) && *arg_ == *(r.arg_);
}

void AggregateExpression::writeTo(Encoder& encoder) const {
    // TODO : verify
    // kind_
    encoder << kind_;

    // name_
    encoder << name_.get();

    // distinct_
    encoder << distinct_;

    // arg_
    if (arg_) {
        encoder << *arg_;
    }
}

void AggregateExpression::resetFrom(Decoder& decoder) {
    // TODO : verify
    // Read name_
    name_ = decoder.readStr();

    // Read arg_
    arg_ = decoder.readExpression();
}

const Value& AggregateExpression::eval(ExpressionContext& ctx) {
    auto iter = AggFun::nameIdMap_.find(name_->c_str());
    if (iter == AggFun::nameIdMap_.end()) {
        return Value::kNullBadData;
    }

    auto val = arg_->eval(ctx);
    fun_.apply(val);

    return 0;
}

std::string AggregateExpression::toString() const {
    std::string arg(arg_->toString());
    std::string isDistinct;
    if (distinct_) { isDistinct = "distinct";}
    std::stringstream out;
    out << *name_ << "(" << isDistinct << " " << arg << ")";
    return out.str();
}

void AggregateExpression::accept(ExprVisitor* visitor) {
    // TODO : impl visitor in nebula-graph
    // visitor->visit(this);
    UNUSED(visitor);
}
}  // namespace nebula
