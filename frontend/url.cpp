#include "url.h"

url::url()
{

}

QString url::getBaseUrl()
{
    return base_url;
}
QString url::base_url{
    "http://localhost:3000/"
};
