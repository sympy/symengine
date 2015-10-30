#include <exception>
#include <symengine/series.h>
#include <symengine/dict.h>

using SymEngine::RCP;
using SymEngine::make_rcp;

namespace SymEngine {

UnivariateSeries::UnivariateSeries(const RCP<const Symbol> &var, const unsigned int &precision, const map_uint_mpz& dict) :
        var_{var}, prec_{precision} {

    map_uint_mpz dict_trunc = {};
    unsigned int max = 0;
    std::copy_if(dict.begin(), dict.end(), std::inserter(dict_trunc, dict_trunc.end()),
        [&](const map_uint_mpz::value_type i)
        {
            if (i.first < prec_) {
                if (max < i.first)
                    max = i.first;
                return true;
            }
            return false;
        } );
    poly_ = univariate_polynomial(var_, max, std::move(dict_trunc));

    SYMENGINE_ASSERT(UnivariateSeries::is_canonical(*poly_, prec_))
}

UnivariateSeries::UnivariateSeries(const RCP<const Symbol> &var, const unsigned int &precision, const std::vector<mpz_class> &v) :
        var_{var}, prec_{precision} {

    std::vector<mpz_class> vtrunc;
    std::copy_if(v.begin(), v.end(), std::back_inserter(vtrunc),
        [&](decltype(v[0]) i) { return i < prec_; } );
    poly_ = UnivariatePolynomial::create(var_, vtrunc);

    SYMENGINE_ASSERT(UnivariateSeries::is_canonical(*poly_, prec_))
}

bool UnivariateSeries::is_canonical(const UnivariatePolynomial& poly, const unsigned int &prec)
{
    return true;
}

std::size_t UnivariateSeries::__hash__() const
{
    return poly_->__hash__() + std::size_t(prec_ * 84728863L);
}

bool UnivariateSeries::__eq__(const Basic &other) const
{
    if (not is_a<UnivariateSeries>(other))
        return false;
    const UnivariateSeries &o = static_cast<const UnivariateSeries &>(other);
    return (eq(*var_, *o.var_) and poly_->__eq__(*o.poly_) and prec_ == o.prec_);
}

int UnivariateSeries::compare(const Basic &other) const
{
    if (not is_a<UnivariateSeries>(other))
        throw std::domain_error("cannot compare with UnivariateSeries");
    const UnivariateSeries &o = static_cast<const UnivariateSeries &>(other);
    return poly_->compare(*o.poly_);
}

std::string UnivariateSeries::__str__() const
{
    std::ostringstream o;
    bool first = true;
    for (auto it : poly_->dict_) {
        if (it.second == 0)
            continue;
        if (first) {
            if (it.second < 0)
                o << "-";
        }
        else {
            if (it.second < 0)
                o << " - ";
            else
                o << " + ";
        }
        first = false;
        if (it.first == 0) {
            o << abs(it.second);
            continue;
        }
        if (abs(it.second) == 1)
            o << var_->get_name();
        else
            o << abs(it.second) << "*" << var_->get_name();
        if (it.first > 1)
            o << "**" << it.first;
    }
    if (o.str() != "0")
        o << " + O(" << var_->get_name() << "**" << prec_ << ")";
    return o.str();
}
} // SymEngine
