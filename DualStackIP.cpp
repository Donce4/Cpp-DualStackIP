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

public:
    DualStackIP(int version, int *parts, string alias)
    {
        this->alias = alias;
        this->version = version;

        if (version == 4)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (parts[i] > 255 || parts[i] < 0)
                {
                    throw invalid_argument("IPv4 was enterred incorrecly");
                }
            }
            this->parts = new int[4];
            for (int i = 0; i < 4; ++i)
            {
                this->parts[i] = parts[i];
            }
        }
        else if (version == 6)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (parts[i] > 65535 || parts[i] < 0)
                {
                    throw invalid_argument("IPv6 was enterred incorrecly");
                }
            }
            this->parts = new int[8];
            for (int i = 0; i < 8; ++i)
            {
                this->parts[i] = parts[i];
            }
        }
        else
        {
            throw invalid_argument("This version of IP doesn't exist");
        }
        uniqueID = nextID;
        ++objectCount;
        ++nextID;
    }

    DualStackIP(int version, string alias)
    {
        this->alias = alias;
        this->version = version;
        if (version == 4)
        {
            this->parts = new int[4];
            for (int i = 0; i < 4; ++i)
            {
                this->parts[i] = 0;
            }
        }
        else if (version == 6)
        {
            this->parts = new int[8];
            for (int i = 0; i < 8; ++i)
            {
                this->parts[i] = 0;
            }
        }
        else
        {
            throw invalid_argument("This version of IP doesn't exist");
        }
        uniqueID = nextID;
        ++objectCount;
        ++nextID;
    }
    ~DualStackIP()
    {
        --objectCount;
        delete[] parts;
    }

public:
    void setAlias(string alias)
    {
        this->alias = alias;
    }
    void setVersion(int version)
    {
        if (version == 4 || version == 6)
        {
            if (this->version != version)
            {
                if (version == 4)
                {
                    int *temp = new int[4];
                    for (int i = 0; i < 4; ++i)
                    {
                        temp[i] = 0;
                    }
                    delete[] parts;
                    parts = temp;
                    this->version = version;
                }
                if (version == 6)
                {
                    int *temp = new int[8];
                    for (int i = 0; i < 8; ++i)
                    {
                        temp[i] = 0;
                    }
                    delete[] parts;
                    parts = temp;
                    this->version = version;
                }
            }
            else
            {
                this->version = version;
            }
        }
        else
        {
            throw invalid_argument("This version of IP doesn't exist");
        }
    }

    void setParts(int *newParts)
    {
        if (this->version == 4)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (newParts[i] > 255 || newParts[i] < 0)
                {
                    throw invalid_argument("IPv4 was enterred incorrecly");
                }
            }
            for (int i = 0; i < 4; ++i)
            {
                this->parts[i] = newParts[i];
            }
        }
        else
        {
            for (int i = 0; i < 8; ++i)
            {
                if (newParts[i] > 65535 || newParts[i] < 0)
                {
                    throw invalid_argument("IPv6 was enterred incorrecly");
                }
            }
            for (int i = 0; i < 8; ++i)
            {
                this->parts[i] = newParts[i];
            }
        }
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

    // Test2

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
    catch (invalid_argument)
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

    assert(DualStackIP::getObjectCount() == 5);

    delete d1;
    delete d2;
    delete d3;
    delete d4;
    assert(d5->getUniqueID() == 4);
    delete d5;
    assert(DualStackIP::getObjectCount() == 0);

    return 0;
}