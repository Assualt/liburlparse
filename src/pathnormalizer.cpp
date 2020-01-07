#include "pathnormalizer.h"
std::string &PathNormalizer::normalizePath(string &path) {
    if (path.empty()) {
        return path;
    }
    path = UrlUtil::decode(path);
    path = sanitizeDotsAndSlashes(path);
    path = UrlUtil::encode(path);
    return path;
}

std::string PathNormalizer::sanitizeDotsAndSlashes(string &path) {
    StringBuilder<char> stringBuilder(path);
    stack<int> slashIndexStack;

    int index = 0;
    while (index < (int)stringBuilder.size() - 1) {
        if (stringBuilder.charAt(index) == '/') {
            slashIndexStack.push(index);
            if (stringBuilder.charAt(index + 1) == '.') {
                if (index < (int)stringBuilder.size() - 2 &&
                    stringBuilder.charAt(index + 2) == '.') {
                    // If it looks like "/../" or ends with "/.."
                    if (((index < (int)stringBuilder.size() - 3) &&
                         (stringBuilder.charAt(index + 3) == '/')) ||
                        (index == (int)stringBuilder.size() - 3)) {
                        bool endOfPath =
                                index == ((int)stringBuilder.size() - 3);
                        slashIndexStack.pop();
                        int endIndex = index + 3;
                        // backtrack so we can detect if this / is part of
                        // another replacement

                        index = slashIndexStack.empty()
                                ? -1
                                : slashIndexStack.top() - 1;
                        slashIndexStack.pop();
                        int startIndex = endOfPath ? index + 1 : index;
                        stringBuilder.Delete(startIndex + 1, endIndex);
                    }
                } else if (
                        ((index < (int)stringBuilder.size() - 2) &&
                         (stringBuilder.charAt(index + 2) == '/')) ||
                        (index == (int)stringBuilder.size() - 2)) {
                    bool endOfPath = index == (int)stringBuilder.size() - 2;
                    slashIndexStack.pop();
                    int startIndex = endOfPath ? index + 1 : index;
                    stringBuilder.Delete(
                            startIndex, index + 2);  // "/./" -> "/"
                    index--;  // backtrack so we can detect if this / is part of
                              // another replacement
                }
            } else if (stringBuilder.charAt(index + 1) == '/') {
                slashIndexStack.pop();
                stringBuilder.DeleteCharAt(index);
                index--;
            }
        }
        index++;
    }
    if (stringBuilder.size() == 0) {
        stringBuilder.Append("/");  // Every path has at least a slash
    }
    return stringBuilder.ToString();
}
