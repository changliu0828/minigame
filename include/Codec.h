#ifndef _CODEC_H_
#define _CODEC_H_

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Endian.h>
#include <muduo/net/TcpConnection.h>

#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using namespace boost::property_tree;

class LengthHeaderCodec;

typedef boost::shared_ptr<LengthHeaderCodec> LengthHeaderCodecPtr;

class LengthHeaderCodec: boost::noncopyable
{
public:
    typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
                                  ptree& ,
                                  muduo::Timestamp)> JsonMessageCallback;

    
    explicit LengthHeaderCodec(const JsonMessageCallback& cb)
        : messageCallback_(cb)
    {
    }

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp receiveTime)
    {
        while (buf->readableBytes() >= kHeaderLen)
        {
            const int32_t len = buf->peekInt32();
            LOG_DEBUG << "Receiven Message Length: " << len;
            if (len > 65536 || len < 0)
            {
                LOG_ERROR << "Invalid length " << len;
                conn->shutdown();  // FIXME: disable reading
                break;
            }
            else if (buf->readableBytes() >= len + kHeaderLen)
            {
                buf->retrieve(kHeaderLen);
                muduo::string message(buf->peek(), len);
                try 
                {
                    std::stringstream stream;
                    stream << message;
                    read_json(stream, jsontree_); 
                }
                catch (json_parser::json_parser_error &e)
                {  
                    LOG_ERROR << "Fiaild to read json string, message: " << message << e.what(); 
                    buf->retrieve(len);
                    return;
                }
                catch (...)
                {
                    LOG_ERROR << "Fiaild to read json string with unknown exception, message: " << message; 
                    buf->retrieve(len);
                    return;
                }
                messageCallback_(conn, jsontree_, receiveTime);
                buf->retrieve(len);
            }
            else
            {
                break;
            }
        } 
    }
    void send(const muduo::net::TcpConnectionPtr &conn,
                const ptree& jsontree)
    {
        muduo::net::Buffer buf(4096);
        std::string  message;
        std::stringstream stream;
        write_json(stream, jsontree);
        message = stream.str();

        buf.append(message.data(), message.size());
        int32_t len = static_cast<int32_t>(message.size());
        LOG_DEBUG << "Send Message Length: " << len;
        int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
        buf.prepend(&be32, sizeof be32);
        conn->send(&buf);
    }

private:
    JsonMessageCallback messageCallback_;
    const static size_t kHeaderLen = sizeof(int32_t);
    boost::property_tree::ptree jsontree_;
};
#endif
