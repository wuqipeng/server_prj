struct RequstInfo
{
    1:i64 uid;
    2:string name;
}

struct ResponseInfo
{
    1:i64 uid;
    2:i32 res_code;
}

service TestService
{
    ResponseInfo GetUserInfo(1:RequstInfo req),
}
