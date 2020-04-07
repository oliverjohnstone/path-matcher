#Path Matcher

This library supports matching regex paths to named parameters

##Dependencies

* [Boost Regex](https://www.boost.org/doc/libs/1_66_0/libs/regex/doc/html/index.html)

##Example

```c++
#include <path_matcher.h>

using namespace std;

vector<string> namedParts;
PathMatcher::Matcher matcher("/test/:arg1/:arg2");

if (matcher.matches("/test/hello/world", namedParts)) {
    // Patch matches and namedParts has been populated with:
    // {{"arg1", "hello}, {"arg2", "world"}}
} else {
    // Patch does not match
}
```