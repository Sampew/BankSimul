#ifndef URL_H
#define URL_H
#include <QString>

class url
{
public:
    url();
    static QString getBaseUrl();

private:
    static QString base_url;
};

#endif // URL_H
