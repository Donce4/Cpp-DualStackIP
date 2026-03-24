#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace std;

class DualStackIP
{
private:
    int version = 0;
    int *parts = nullptr;
    int uniqueID = 0;
    static int objectCount;
    static int nextID;

public:
    string alias = "";

public:
    DualStackIP(int version, string alias)
    {

        setVersion(version);
        setAlias(alias);
        assignID();
    }

    // Delegating Constructor: Calls the constructor above, then just sets the parts
    DualStackIP(int version, int *parts, string alias) : DualStackIP(version, alias)
    {
        setParts(parts);
    }


    void setAlias(string alias)
    {
        this->alias = alias;
    }

    ~DualStackIP()
    {
        --objectCount;
        delete[] parts;
    }
    void setVersion(int version)
    {
        if (version != 4 && version != 6)
            throw invalid_argument("This version of IP doesn't exist");

        if (this->version != version)
        {
            delete[] parts;
            this->version = version;
            allocateParts();
        }
    }

    void setParts(int *newParts)
    {
        validateParts(this->version, newParts);
        int size = getPartsSize();
        for (int i = 0; i < size; ++i)
        {
            this->parts[i] = newParts[i];
        }
    }

    static int
    getObjectCount()
    {
        return objectCount;
    }
    int *getParts()
    {
        return parts;
    }
    int getUniqueID()
    {
        return uniqueID;
    }
    string getAlias()
    {
        return alias;
    }
    int getVersion()
    {
        return version;
    }

private: // Helper functions
    int getPartsSize()
    {
        return (version == 4) ? 4 : 8;
    }

    void allocateParts()
    {
        int size = getPartsSize();
        parts = new int[size];
        for (int i = 0; i < size; ++i)
        {
            parts[i] = 0;
        }
    }

    void validateParts(int v, int *newParts)
    {
        int size = (v == 4) ? 4 : 8;
        int maxVal = (v == 4) ? 255 : 65535;
        string errorMsg = (v == 4) ? "IPv4 was enterred incorrecly" : "IPv6 was enterred incorrecly";

        for (int i = 0; i < size; ++i)
        {
            if (newParts[i] > maxVal || newParts[i] < 0)
            {
                throw invalid_argument(errorMsg);
            }
        }
    }

    void assignID()
    {
        uniqueID = nextID;
        ++objectCount;
        ++nextID;
    }

public:
    string toString()
    {
        stringstream ss;
        // 1. Common Prefix
        ss << getAlias() << " " << getVersion() << " ";
        // IPv4 formating (with '.')
        if (getVersion() == 4)
        {
            ss << getParts()[0] << "." << getParts()[1] << "." << getParts()[2] << "." << getParts()[3];
        }
        // IPv6 formating (with ':')
        else
        {
            ss << getParts()[0] << ":" << getParts()[1] << ":" << getParts()[2] << ":" << getParts()[3] << ":"
               << getParts()[4] << ":" << getParts()[5] << ":" << getParts()[6] << ":" << getParts()[7];
        }
        ss << " " << getUniqueID() << endl;
        return ss.str();
    }
};

int DualStackIP::objectCount = 0;
int DualStackIP::nextID = 0;

int main()
{
    // Declared pointers OUTSIDE to ensure visibility for unified cleanup.
    DualStackIP *d1 = nullptr;
    DualStackIP *d2 = nullptr;
    DualStackIP *d3 = nullptr;
    DualStackIP *d4 = nullptr;
    DualStackIP *d5 = nullptr;
    DualStackIP *failHost = nullptr;

    try
    {
        int ip1[4] = {192, 168, 1, 1};
        d1 = new DualStackIP(4, ip1, "Host1");

        // Test 2
        d1->alias = "Host2";
        assert(d1->getAlias() == "Host2");

        int ip2[4] = {172, 16, 254, 1};
        d1->setParts(ip2);

        for (int i = 0; i < 4; ++i)
        {
            assert(d1->getParts()[i] == ip2[i]);
        }

        d1->setVersion(6);
        assert(d1->getVersion() == 6);

        // Test 4 and 5 (Creating valid objects)
        int ip3[4] = {192, 168, 1, 1};
        d2 = new DualStackIP(4, ip3, "Host2");

        int ip4[8] = {8193, 18528, 18528, 0, 0, 0, 0, 34952};
        d3 = new DualStackIP(6, ip4, "Host3");

        int ip5[4] = {10, 0, 0, 15};
        d4 = new DualStackIP(4, ip5, "Host4");

        int ip6[4] = {212, 59, 1, 100};
        d5 = new DualStackIP(4, ip6, "Host5");

        assert(DualStackIP::getObjectCount() == 5);

        // INTENTIONAL EXCEPTION
        int badIp[4] = {999, 999, 999, 999};
        failHost = new DualStackIP(4, badIp, "FailHost"); // This throws an exception
    }
    catch (const invalid_argument &e)
    {
    }
    catch (...)
    {
    }

    DualStackIP *allPointers[] = {d1, d2, d3, d4, d5, failHost};
    for (int i = 0; i < 6; ++i)
    {
        delete allPointers[i];
    }

    assert(DualStackIP::getObjectCount() == 0);
    return 0;
}
