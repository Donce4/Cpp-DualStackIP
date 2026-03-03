#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace std;

class DualStackIP
{
private:
    int version;
    int *parts;
    string alias;
    int uniqueID;
    static int objectCount;
    static int nextID;

    // New helper functions

    void allocateParts(int v)
    {
        int size = (v == 4) ? 4 : 8;
        this->parts = new int[size];
        for (int i = 0; i < size; ++i)
        {
            this->parts[i] = 0;
        }
    }

    void validateAndAssign(int v, int *newParts)
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
        for (int i = 0; i < size; ++i)
        {
            this->parts[i] = newParts[i];
        }
    }

    void assignID()
    {
        uniqueID = nextID;
        ++objectCount;
        ++nextID;
    }

public:
    DualStackIP(int version, int *parts, string alias)
    {
        if (version != 4 && version != 6)
            throw invalid_argument("This version of IP doesn't exist");

        this->version = version;
        this->alias = alias;

        allocateParts(version);
        validateAndAssign(version, parts);
        assignID();
    }

    DualStackIP(int version, string alias)
    {
        if (version != 4 && version != 6)
            throw invalid_argument("This version of IP doesn't exist");

        this->version = version;
        this->alias = alias;

        allocateParts(version);
        assignID();
    }

    ~DualStackIP()
    {
        --objectCount;
        delete[] parts;
    }

    void setAlias(string alias)
    {
        this->alias = alias;
    }

    void setVersion(int version)
    {
        if (version != 4 && version != 6)
            throw invalid_argument("This version of IP doesn't exist");

        if (this->version != version)
        {
            delete[] parts;
            this->version = version;
            allocateParts(version);
        }
    }

    void setParts(int *newParts)
    {
        validateAndAssign(this->version, newParts);
    }

    static int getObjectCount()
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

    string toString()
    {
        stringstream ss;
        if (getVersion() == 4)
        {
            ss << getAlias() << " " << getVersion() << " " << getParts()[0] << "." << getParts()[1] << "." << getParts()[2] << "." << getParts()[3] << " " << getUniqueID() << endl;
        }
        else
        {
            ss << getAlias() << " " << getVersion() << " " << getParts()[0] << ":" << getParts()[1] << ":" << getParts()[2] << ":" << getParts()[3] << ":" << getParts()[4] << ":" << getParts()[5] << ":" << getParts()[6] << ":" << getParts()[7] << " " << getUniqueID() << endl;
        }
        return ss.str();
    }
};

int DualStackIP::objectCount = 0;
int DualStackIP::nextID = 0;

int main()
{
    int ip1[4] = {192, 168, 1, 1};
    DualStackIP *d1 = new DualStackIP(4, ip1, "Host1");

    // Test 1
    assert(d1->getVersion() == 4);
    assert(d1->getAlias() == "Host1");
    assert(d1->getUniqueID() == 0);
    assert(d1->getParts()[0] == ip1[0]);
    assert(d1->getParts()[1] == ip1[1]);
    assert(d1->getParts()[2] == ip1[2]);
    assert(d1->getParts()[3] == ip1[3]);
    assert(d1->toString() == "Host1 4 192.168.1.1 0\n");

    // Test 2
    d1->setAlias("Host2");
    assert(d1->getAlias() == "Host2");

    int ip2[4] = {172, 16, 254, 1};
    d1->setParts(ip2);
    assert(d1->getParts()[0] == ip2[0]);
    assert(d1->getParts()[1] == ip2[1]);
    assert(d1->getParts()[2] == ip2[2]);
    assert(d1->getParts()[3] == ip2[3]);

    d1->setVersion(6);
    assert(d1->getVersion() == 6);
    assert(d1->getParts()[0] == 0);
    assert(d1->getParts()[1] == 0);
    assert(d1->getParts()[2] == 0);
    assert(d1->getParts()[3] == 0);
    assert(d1->getParts()[4] == 0);
    assert(d1->getParts()[5] == 0);
    assert(d1->getParts()[6] == 0);
    assert(d1->getParts()[7] == 0);

    // Test 3
    bool exceptionThrown = false;
    try
    {
        d1->setVersion(5);
    }

    catch (const invalid_argument &e)
    {
        exceptionThrown = true;
    }
    catch (...) // New
    {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // Test 4 and 5
    int ip3[4] = {192, 168, 1, 1};
    DualStackIP *d2 = new DualStackIP(4, ip3, "Host2");
    int ip4[8] = {8193, 18528, 18528, 0, 0, 0, 0, 34952};
    DualStackIP *d3 = new DualStackIP(6, ip4, "Host3");
    int ip5[4] = {10, 0, 0, 15};
    DualStackIP *d4 = new DualStackIP(4, ip5, "Host4");
    int ip6[4] = {212, 59, 1, 100};
    DualStackIP *d5 = new DualStackIP(4, ip6, "Host5");

    // New
    assert(DualStackIP::getObjectCount() == 5);

    int objectsBeforeError = DualStackIP::getObjectCount();
    bool creationFailed = false;
    try
    {
        int badIp[4] = {999, 999, 999, 999};
        DualStackIP *failHost = new DualStackIP(4, badIp, "FailHost");
        delete failHost;
    }
    catch (const invalid_argument &e)
    {
        creationFailed = true;
    }
    catch (...)
    {
        creationFailed = true;
    }
    assert(creationFailed);

    assert(DualStackIP::getObjectCount() == objectsBeforeError);

    delete d1;
    delete d2;
    delete d3;
    delete d4;
    assert(d5->getUniqueID() == 4);
    delete d5;

    assert(DualStackIP::getObjectCount() == 0);

    return 0;
}
