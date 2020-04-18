#include <vector>
using std::vector;

template <typename T>
class Version {
 private:
    T value;
    int64_t previousVersion;
 public:
    Version() : value(NULL), previousVersion(NULL) {}
    Version(T value, int64_t previousVersion) :
        value(value), previousVersion(previousVersion) {}
    Version(const Version& version) :
        value(version.value), previousVersion(version.previousVersion) {}
    T getValue() {
        return value;
    }
    int64_t getPreviousVersion() {
        return previousVersion;
    }
};

template <typename T>
class PersistentStack {
 private:
    vector<Version<T>> versions;
 public:
    PersistentStack() {
        versions.resize(1);
    }
    void push(T value, int64_t version) {
		if (version >= versions.size())
			throw std::exception("Persistent stack out of range");
        versions.emplace_back(value, version);
    }
    T pop(int64_t version) {
		if (version >= versions.size())
			throw std::exception("Persistent stack out of range");
        Version<T> current = versions[version];
        Version<T> previous = versions[current.getPreviousVersion()];
        versions.emplace_back(previous);
        return current.getValue();
    }
};
