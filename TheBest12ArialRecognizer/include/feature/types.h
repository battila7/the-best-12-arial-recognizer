#ifndef FEATURE_TYPES_H
#define FEATURE_TYPES_H

#include<map>
#include<vector>

namespace arialrec
{

namespace feature
{

using feature_t = int;

using FeatureVector = std::vector<feature_t>;

using FeatureMap = std::map<char, FeatureVector>;

} // namespace feature

} // namespace arialrec

#endif // FEATURE_TYPES_H