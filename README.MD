# Path Matcher

This library supports matching regex paths to named parameters.

Inspired by / copied from [path-to-regexp](https://github.com/pillarjs/path-to-regexp)

## Dependencies

* [Boost Regex](https://www.boost.org/doc/libs/1_66_0/libs/regex/doc/html/index.html)

## Example

```c++
#include <path_matcher.h>
#include <iostream>

using namespace std;
using namespace PathMatcher;

ArgResults namedParts;
Matcher matcher("/test/:arg1/:arg2");

if (matcher.matches("/test/hello/world", namedParts)) {
    // Path matches and namedParts has been populated with:
    // {{"arg1", "hello}, {"arg2", "world"}}

    cout << "arg1: " << namedParts["arg1"] << endl;
    cout << "arg2: " << namedParts["arg2"] << endl;
} else {
    // Path does not match
}
```