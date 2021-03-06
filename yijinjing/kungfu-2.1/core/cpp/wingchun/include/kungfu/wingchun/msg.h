//
// Created by Keren Dong on 2019-06-19.
//

#ifndef WINGCHUN_EVENT_H
#define WINGCHUN_EVENT_H

#include <cinttypes>
#include <cmath>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <kungfu/wingchun/common.h>
#include <kungfu/yijinjing/journal/journal.h>

namespace kungfu
{
    namespace wingchun
    {
        namespace msg
        {
            namespace type
            {
                enum MsgType
                {
                    Quote = 101,
                    Entrust = 102,
                    Transaction = 103,

                    Bar = 110,

                    OrderInput = 201,
                    OrderAction = 202,
                    Order = 203,
                    Trade = 204,
                    Position = 205,
                    Asset = 206,
                    AssetSnapshot = 207,
                    PositionDetail = 208,
                    Instrument = 209,
                    AlgoOrderInput = 210,
                    AlgoOrderReport = 211,
                    AlgoOrderModify = 212,
                    OrderActionError = 213,

                    Subscribe = 302,
                    SubscribeAll = 303,

                    NewOrderSingle = 353,
                    CancelOrder = 354,
                    CancelAllOrder = 355,
                    InstrumentRequest = 356,

                    BrokerStateRefresh = 400,
                    BrokerState = 401,
                    QryAsset = 402,
                    PublishAllAssetInfo = 403,
                    RemoveStrategy = 404,
                    
                    PositionEnd = 800,
                    PositionDetailEnd = 801,
                    InstrumentEnd = 802,
                };
            }

            namespace data
            {
                enum class BrokerState : int
                {
                    Unknown = 0,
                    Idle = 1,
                    DisConnected = 2,
                    Connected = 3,
                    LoggedIn = 4,
                    LoggedInFailed = 5,
                    Ready = 100
                };

#ifdef _WIN32
    #pragma pack(push, 1)
#endif
                struct Instrument
                {
                    char instrument_id[INSTRUMENT_ID_LEN];     //??????ID
                    char exchange_id[EXCHANGE_ID_LEN];         //?????????ID
                    InstrumentType instrument_type;            //????????????

                    char product_id[PRODUCT_ID_LEN];           //??????ID

                    int contract_multiplier;                   //????????????
                    double price_tick;                         //??????????????????

                    char open_date[DATE_LEN];                  //?????????
                    char create_date[DATE_LEN];                //?????????
                    char expire_date[DATE_LEN];                //?????????

                    int delivery_year;                         //????????????
                    int delivery_month;                        //?????????

                    bool is_trading;                           //??????????????????

                    double long_margin_ratio;                  //??????????????????
                    double short_margin_ratio;                 //??????????????????

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    {strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN); }

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    {strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN); }

                    const std::string get_product_id() const
                    { return std::string(product_id); }

                    void set_product_id(const std::string& product_id)
                    {strncpy(this->product_id, product_id.c_str(), PRODUCT_ID_LEN);}

                    const std::string get_open_date() const
                    { return std::string(open_date); }

                    void set_open_date(const std::string& open_date)
                    {strncpy(this->open_date, open_date.c_str(), DATE_LEN);}

                    const std::string get_create_date() const
                    { return std::string(create_date); }

                    void set_create_date(const std::string& create_date)
                    {strncpy(this->create_date, create_date.c_str(), DATE_LEN);}

                    const std::string get_expire_date() const
                    { return std::string(expire_date); }

                    void set_expire_date(const std::string& expire_date)
                    {strncpy(this->expire_date, expire_date.c_str(), DATE_LEN);}

                    bool operator ==(const Instrument & obj) const
                    {
                        return strcmp(this->instrument_id, obj.instrument_id) == 0 && strcmp(this->exchange_id, obj.exchange_id) == 0;
                    }

                    bool operator <(const Instrument & obj) const
                    {
                        return get_symbol_id(this->get_instrument_id(), this->get_exchange_id()) < get_symbol_id(obj.get_instrument_id(), obj.get_exchange_id());
                    }

#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Instrument &instrument)
                {
                    j["exchange_id"] = std::string(instrument.exchange_id);
                    j["instrument_id"] = std::string(instrument.instrument_id);
                    j["instrument_type"] = instrument.instrument_type;
                    j["product_id"] = std::string(instrument.product_id);
                    j["contract_multiplier"] = instrument.contract_multiplier;
                    j["price_tick"] = instrument.price_tick;
                    j["open_date"] = std::string(instrument.open_date);
                    j["create_date"] = std::string(instrument.create_date);
                    j["expire_date"] = std::string(instrument.expire_date);
                    j["delivery_year"] = instrument.delivery_year;
                    j["delivery_month"] = instrument.delivery_month;
                    j["long_margin_ratio"] = instrument.long_margin_ratio;
                    j["short_margin_ratio"] = instrument.short_margin_ratio;
                }

                //??????
                struct Quote
                {
                    char source_id[SOURCE_ID_LEN];              //??????ID
                    char trading_day[DATE_LEN];                 //?????????

                    int64_t data_time;                          //??????????????????

                    char instrument_id[INSTRUMENT_ID_LEN];      //??????ID
                    char exchange_id[EXCHANGE_ID_LEN];          //?????????ID

                    InstrumentType instrument_type;             //????????????

                    double pre_close_price;                     //?????????
                    double pre_settlement_price;                //?????????

                    double last_price;                          //?????????
                    int64_t volume;                             //??????
                    double turnover;                            //????????????

                    double pre_open_interest;                   //????????????
                    double open_interest;                       //?????????

                    double open_price;                          //?????????
                    double high_price;                          //?????????
                    double low_price;                           //?????????

                    double upper_limit_price;                   //????????????
                    double lower_limit_price;                   //????????????

                    double close_price;                         //?????????
                    double settlement_price;                    //?????????

                    double bid_price[10];                       //?????????
                    double ask_price[10];                       //?????????
                    int64_t bid_volume[10];                     //?????????
                    int64_t ask_volume[10];                     //?????????

                    std::string get_source_id() const
                    { return std::string(source_id); }

                    void set_source_id(const std::string& source_id)
                    {strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN);}

                    std::string get_trading_day() const
                    { return std::string(trading_day); }

                    void set_trading_day(const std::string& trading_day)
                    {strncpy(this->trading_day, trading_day.c_str(), DATE_LEN);}

                    std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN);}

                    std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    {strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN);}

                    std::vector<double> get_bid_price() const
                    { return std::vector<double>(bid_price, bid_price + 10); }

                    void set_bid_price(const std::vector<double> &bp)
                    { memcpy(bid_price, (const void*) bp.data(), sizeof(double) * std::min(10, int(bp.size())));}

                    std::vector<double> get_ask_price() const
                    { return std::vector<double>(ask_price, ask_price + 10); }

                    void set_ask_price(const std::vector<double> &ap)
                    { memcpy(ask_price, (const void*) ap.data(), sizeof(double) * std::min(10, int(ap.size())));}

                    std::vector<int64_t> get_bid_volume() const
                    { return std::vector<int64_t>(bid_volume, bid_volume + 10); }

                    void set_bid_volume(const std::vector<int64_t> &bv)
                    { memcpy(bid_volume, (const void*) bv.data(), sizeof(int64_t) * std::min(10, int(bv.size())));}

                    std::vector<int64_t> get_ask_volume() const
                    { return std::vector<int64_t>(ask_volume, ask_volume + 10); }

                    void set_ask_volume(const std::vector<int64_t> &av)
                    { memcpy(ask_volume, (const void*) av.data(), sizeof(int64_t) * std::min(10, int(av.size())));}

#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Quote &quote)
                {
                    j["trading_day"] = quote.get_trading_day();
                    j["data_time"] = quote.data_time;
                    j["instrument_id"] = quote.get_instrument_id();
                    j["exchange_id"] = quote.get_exchange_id();
                    j["instrument_type"] = quote.instrument_type;

                    j["pre_close_price"] = quote.pre_close_price;
                    j["pre_settlement_price"] = quote.pre_settlement_price;

                    j["last_price"] = quote.last_price;
                    j["volume"] = quote.volume;
                    j["turnover"] = quote.turnover;

                    j["pre_open_interest"] = quote.pre_open_interest;
                    j["open_interest"] = quote.open_interest;

                    j["open_price"] = quote.open_price;
                    j["high_price"] = quote.high_price;
                    j["low_price"] = quote.low_price;

                    j["upper_limit_price"] = quote.upper_limit_price;
                    j["lower_limit_price"] = quote.lower_limit_price;

                    j["close_price"] = quote.close_price;
                    j["settlement_price"] = quote.settlement_price;

                    j["bid_price"] = quote.get_bid_price();
                    j["ask_price"] = quote.get_ask_price();

                    j["bid_volume"] = quote.get_bid_volume();
                    j["ask_volume"] = quote.get_ask_volume();
                }

                inline void from_json(const nlohmann::json &j, Quote &quote)
                {
                    quote.set_trading_day(j["trading_day"].get<std::string>());
                    quote.data_time = j["data_time"];
                    quote.set_instrument_id(j["instrument_id"].get<std::string>());
                    quote.set_exchange_id(j["exchange_id"].get<std::string>());
                    quote.instrument_type = j["instrument_type"];
                    quote.pre_close_price = j["pre_close_price"];
                    quote.pre_settlement_price = j["pre_settlement_price"];
                    quote.last_price = j["last_price"];
                    quote.volume = j["volume"];
                    quote.turnover = j["turnover"];
                    quote.pre_open_interest = j["pre_open_interest"];
                    quote.open_interest = j["open_interest"];

                    quote.open_price = j["open_price"];
                    quote.high_price = j["high_price"];
                    quote.low_price = j["low_price"];

                    quote.upper_limit_price = j["upper_limit_price"];
                    quote.lower_limit_price = j["lower_limit_price"];

                    quote.close_price = j["close_price"];
                    quote.settlement_price = j["settlement_price"];

                    quote.set_bid_price(j["bid_price"].get<std::vector<double>>());
                    quote.set_ask_price(j["ask_price"].get<std::vector<double>>());
                    quote.set_bid_volume(j["bid_volume"].get<std::vector<int64_t>>());
                    quote.set_ask_volume(j["ask_volume"].get<std::vector<int64_t>>());
                }

                //????????????
                struct Entrust
                {
                    char source_id[SOURCE_ID_LEN];              //??????ID
                    char trading_day[DATE_LEN];                 //?????????

                    int64_t data_time;                          //??????????????????

                    char instrument_id[INSTRUMENT_ID_LEN];      //??????ID
                    char exchange_id[EXCHANGE_ID_LEN];          //???????????????

                    InstrumentType instrument_type;             //????????????

                    double price;                               //????????????
                    int64_t volume;                             //?????????
                    Side side;                                  //????????????
                    PriceType price_type;                       //??????????????????????????????????????????????????????

                    int64_t main_seq;                           //?????????
                    int64_t seq;                                //?????????

                    const std::string get_source_id() const
                    { return std::string(source_id); }

                    void set_source_id(const std::string& source_id)
                    {strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN);}

                    const std::string get_trading_day() const
                    { return std::string(trading_day); }

                    void set_trading_day(const std::string& trading_day)
                    {strncpy(this->trading_day, trading_day.c_str(), DATE_LEN);}

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN);}

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    {strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN);}
#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Entrust &entrust)
                {
                    j["source_id"] = std::string(entrust.source_id);
                    j["trading_day"] = std::string(entrust.trading_day);
                    j["data_time"] = entrust.data_time;
                    j["instrument_id"] = std::string(entrust.instrument_id);
                    j["exchange_id"] = std::string(entrust.exchange_id);
                    j["instrument_type"] = entrust.instrument_type;
                    j["price"] = entrust.price;
                    j["volume"] = entrust.volume;
                    j["side"] = entrust.side;
                    j["price_type"] = entrust.price_type;
                    j["main_seq"] = entrust.main_seq;
                    j["seq"] = entrust.seq;
                }

                //????????????
                struct Transaction
                {
                    char source_id[SOURCE_ID_LEN];              //??????ID
                    char trading_day[DATE_LEN];                 //?????????

                    int64_t data_time;                          //??????????????????

                    char instrument_id[INSTRUMENT_ID_LEN];      //??????ID
                    char exchange_id[EXCHANGE_ID_LEN];          //???????????????

                    InstrumentType instrument_type;             //????????????

                    double price;                               //?????????
                    int64_t volume;                             //?????????

                    int64_t bid_no;                             //???????????????
                    int64_t ask_no;                             //???????????????

                    ExecType exec_type;                         //SZ: ????????????
                    BsFlag bs_flag;                            //SH: ???????????????

                    int64_t main_seq;                               //?????????
                    int64_t seq;                                    //?????????

                    const std::string get_source_id() const
                    { return std::string(source_id); }

                    void set_source_id(const std::string& source_id)
                    {strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN);}

                    const std::string get_trading_day() const
                    { return std::string(trading_day); }

                    void set_trading_day(const std::string& trading_day)
                    {strncpy(this->trading_day, trading_day.c_str(), DATE_LEN);}

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN);}

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    {strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN);}

#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Transaction &transaction)
                {
                    j["source_id"] = std::string(transaction.source_id);
                    j["trading_day"] = std::string(transaction.trading_day);
                    j["data_time"] = transaction.data_time;
                    j["instrument_id"] = std::string(transaction.instrument_id);
                    j["exchange_id"] = std::string(transaction.exchange_id);
                    j["instrument_type"] = transaction.instrument_type;
                    j["price"] = transaction.price;
                    j["volume"] = transaction.volume;
                    j["bid_no"] = transaction.bid_no;
                    j["ask_no"] = transaction.ask_no;
                    j["exec_type"] = transaction.exec_type;
                    j["bs_flag"] = transaction.bs_flag;
                    j["main_seq"] = transaction.main_seq;
                    j["seq"] = transaction.seq;
                }

                struct Bar
                {
                    char trading_day[DATE_LEN];            //?????????
                    char instrument_id[INSTRUMENT_ID_LEN]; //????????????
                    char exchange_id[EXCHANGE_ID_LEN];     //???????????????

                    int64_t start_time;                    //????????????
                    int64_t end_time;                      //????????????

                    double open;                           //???
                    double close;                          //???
                    double low;                            //???
                    double high;                           //???

                    int64_t volume;                        //???????????????
                    int64_t start_volume;                  //??????????????????

                    int32_t tick_count;                    //????????????tick???

                    const std::string get_trading_day() const
                    { return std::string(trading_day); }

                    void set_trading_day(const std::string& trading_day)
                    { strncpy(this->trading_day, trading_day.c_str(), DATE_LEN); }

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN);}

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    {strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN);}
#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif



                inline void to_json(nlohmann::json &j, const Bar &bar)
                {
                    j["trading_day"] = std::string(bar.trading_day);
                    j["instrument_id"] = std::string(bar.instrument_id);
                    j["exchange_id"] = std::string(bar.exchange_id);
                    j["start_time"] = bar.start_time;
                    j["end_time"] = bar.end_time;
                    j["open"] = bar.open;
                    j["close"] = bar.close;
                    j["low"] = bar.low;
                    j["high"] = bar.high;
                    j["volume"] = bar.volume;
                    j["start_volume"] = bar.start_volume;
                    j["tick_count"] = bar.tick_count;
                }

                //??????????????????
                struct InstrumentCommissionRate
                {
                    char instrument_id[INSTRUMENT_ID_LEN];  //????????????
                    char exchange_id[EXCHANGE_ID_LEN];      //???????????????

                    InstrumentType instrument_type;         //????????????

                    char account_id[ACCOUNT_ID_LEN];        //??????ID
                    char broker_id[BROKER_ID_LEN];          //Broker ID

                    CommissionRateMode mode;                //???????????????(??????????????????????????????)

                    double open_ratio;                      //????????????
                    double close_ratio;                     //????????????
                    double close_today_ratio;               //????????????

                    double min_commission;                  //???????????????

                    std::string get_instrument_id()
                    { return std::string(instrument_id); }

                    std::string get_exchange_id()
                    { return std::string(exchange_id); }

                    std::string get_account_id()
                    { return std::string(account_id); }

                    std::string get_broker_id()
                    { return std::string(broker_id); }

#ifndef _WIN32
                } __attribute__((packed));

#else
                };
#endif

                //????????????
                struct OrderInput
                {
                    uint64_t order_id;                       //??????ID

                    char instrument_id[INSTRUMENT_ID_LEN];   //????????????
                    char exchange_id[EXCHANGE_ID_LEN];       //???????????????

                    char source_id[SOURCE_ID_LEN];           //??????ID
                    char account_id[ACCOUNT_ID_LEN];         //??????ID

                    InstrumentType instrument_type;          //????????????

                    double limit_price;                      //??????
                    double frozen_price;                     //????????????

                    int64_t volume;                          //??????

                    Side side;                               //????????????
                    Offset offset;                           //????????????
                    HedgeFlag hedge_flag;                    //??????????????????
                    PriceType price_type;                    //????????????
                    VolumeCondition volume_condition;        //???????????????
                    TimeCondition time_condition;            //??????????????????

                    uint64_t parent_id;                      //?????????ID

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN);}

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    {strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN);}

                    const std::string get_account_id() const
                    { return std::string(account_id); }

                    void set_account_id(const std::string &account_id)
                    { strncpy(this->account_id, account_id.c_str(), ACCOUNT_ID_LEN);}

                    const std::string get_source_id() const
                    { return std::string(source_id); }

                    void set_source_id(const std::string& source_id)
                    { strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN);}
#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const OrderInput &input)
                {
                    j["order_id"] = input.order_id;
                    j["instrument_id"] = std::string(input.instrument_id);
                    j["exchange_id"] = std::string(input.exchange_id);
                    j["account_id"] = std::string(input.account_id);
                    j["source_id"] = std::string(input.source_id);
                    j["instrument_type"] = input.instrument_type;
                    j["volume"] = input.volume;
                    j["limit_price"] = input.limit_price;
                    j["frozen_price"] =  input.frozen_price;
                    j["side"] = input.side;
                    j["offset"] = input.offset;
                    j["hedge_flag"] = input.hedge_flag;
                    j["price_type"] = input.price_type;
                    j["volume_condition"] = input.volume_condition;
                    j["time_condition"] = input.time_condition;
                    j["parent_id"] = input.parent_id;
                }

                inline void from_json(const nlohmann::json &j, OrderInput &input)
                {
                    input.order_id = j["order_id"].get<uint64_t>();
                    strncpy(input.instrument_id, j["instrument_id"].get<std::string>().c_str(), INSTRUMENT_ID_LEN);
                    strncpy(input.exchange_id, j["exchange_id"].get<std::string>().c_str(), EXCHANGE_ID_LEN);
                    strncpy(input.account_id, j["account_id"].get<std::string>().c_str(), ACCOUNT_ID_LEN);
                    strncpy(input.source_id, j["source_id"].get<std::string>().c_str(), SOURCE_ID_LEN);
                    input.instrument_type = j["instrument_type"];
                    input.limit_price = j["limit_price"].get<double>();
                    input.frozen_price = j["frozen_price"].get<double>();
                    input.volume = j["volume"].get<int64_t>();
                    input.side = j["side"];
                    input.offset = j["offset"];
                    input.hedge_flag = j["hedge_flag"];
                    input.price_type = j["price_type"];
                    input.volume_condition = j["volume_condition"];
                    input.time_condition = j["time_condition"];
                    input.parent_id = j["parent_id"].get<uint64_t>();
                }

                //????????????
                struct OrderAction
                {
                    uint64_t order_id;                       //??????ID
                    uint64_t order_action_id;                //????????????ID

                    OrderActionFlag action_flag;             //??????????????????

                    double price;                            //??????
                    int64_t volume;                          //??????
#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const OrderAction &action)
                {
                    j["order_id"] = action.order_id;
                    j["order_action_id"] = action.order_action_id;
                    j["action_flag"] = action.action_flag;
                    j["price"] = action.price;
                    j["volume"] = action.volume;
                }

                inline void from_json(const nlohmann::json &j, OrderAction &action)
                {
                    action.order_id = j["order_id"].get<uint64_t>();
                    action.order_action_id = j["order_action_id"].get<uint64_t>();
                    action.action_flag = j["action_flag"];
                    action.price = j["price"].get<double>();
                    action.volume = j["volume"].get<int64_t>();
                }

                //??????????????????
                struct OrderActionError
                {
                    uint64_t order_id;                       //??????ID
                    uint64_t order_action_id;                //????????????ID
                    int32_t error_id;                        //??????ID
                    char error_msg[ERROR_MSG_LEN];           //????????????

                    const std::string get_error_msg() const { return std::string(error_msg); }
                    void set_error_msg(const std::string &msg) { strncpy(this->error_msg, msg.c_str(), ERROR_MSG_LEN); }
#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const OrderActionError &error)
                {
                    j["order_id"] = error.order_id;
                    j["order_action_id"] = error.order_action_id;
                    j["error_id"] = error.error_id;
                    j["error_msg"] = error.error_msg;
                }

                inline void from_json(const nlohmann::json &j, OrderActionError &error)
                {
                    error.order_id = j["order_id"].get<uint64_t>();
                    error.order_action_id = j["order_action_id"].get<uint64_t>();
                    error.error_id = j["error_id"];
                    error.set_error_msg(j["error_msg"].get<std::string>());
                }

                //????????????
                struct Order
                {
                    uint64_t parent_id;                      //?????????ID
                    uint64_t order_id;                       //??????ID

                    int64_t insert_time;                     //??????????????????
                    int64_t update_time;                     //??????????????????

                    char trading_day[DATE_LEN];              //?????????

                    char instrument_id[INSTRUMENT_ID_LEN];   //??????ID
                    char exchange_id[EXCHANGE_ID_LEN];       //?????????ID

                    char source_id[SOURCE_ID_LEN];           //??????ID
                    char account_id[ACCOUNT_ID_LEN];         //??????ID
                    char client_id[CLIENT_ID_LEN];           //Client ID

                    InstrumentType instrument_type;          //????????????

                    double limit_price;                      //??????
                    double frozen_price;                     //???????????????????????????????????????0

                    int64_t volume;                          //??????
                    int64_t volume_traded;                   //????????????
                    int64_t volume_left;                     //????????????

                    double tax;                              //???
                    double commission;                       //?????????

                    OrderStatus status;                      //????????????

                    int32_t error_id;                        //??????ID
                    char error_msg[ERROR_MSG_LEN];           //????????????

                    Side side;                               //????????????
                    Offset offset;                           //????????????
                    HedgeFlag hedge_flag;                    //??????????????????
                    PriceType price_type;                    //????????????
                    VolumeCondition volume_condition;        //???????????????
                    TimeCondition time_condition;            //??????????????????

                    const std::string get_trading_day() const
                    { return std::string(trading_day); }

                    void set_trading_day(const std::string& trading_day)
                    { strncpy(this->trading_day, trading_day.c_str(), DATE_LEN);}

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN); }

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    void set_exchange_id(const std::string& exchange_id)
                    { strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN); }

                    const std::string get_account_id() const
                    { return std::string(account_id); }

                    void set_account_id(const std::string& account_id)
                    { strncpy(this->account_id, account_id.c_str(), ACCOUNT_ID_LEN); }

                    const std::string get_source_id() const
                    { return std::string(source_id);}

                    void set_source_id(const std::string& source_id)
                    { strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN); }

                    const std::string get_client_id() const
                    { return std::string(client_id); }

                    void set_client_id(const std::string& client_id)
                    { strncpy(this->client_id, client_id.c_str(), CLIENT_ID_LEN); }

                    const std::string get_error_msg() const
                    { return std::string(error_msg); }

                    void set_error_msg(const std::string& error_msg)
                    { strncpy(this->error_msg, error_msg.c_str(), ERROR_MSG_LEN); }
#ifndef _WIN32
                } __attribute__((packed));

#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Order &order)
                {

                    j["order_id"] = std::to_string(order.order_id);
                    j["insert_time"] = order.insert_time;
                    j["update_time"] = order.update_time;
                    j["trading_day"] = std::string(order.trading_day);

                    j["instrument_id"] = std::string(order.instrument_id);
                    j["exchange_id"] = std::string(order.exchange_id);

                    j["source_id"] = std::string(order.source_id);
                    j["account_id"] = std::string(order.account_id);
                    j["client_id"] = std::string(order.client_id);

                    j["instrument_type"] = order.instrument_type;

                    j["limit_price"] = order.limit_price;
                    j["frozen_price"] = order.frozen_price;

                    j["volume"] = order.volume;
                    j["volume_traded"] = order.volume_traded;
                    j["volume_left"] = order.volume_left;

                    j["tax"] = order.tax;
                    j["commission"] = order.commission;

                    j["error_id"] = order.error_id;
                    j["error_msg"] = std::string(order.error_msg);

                    j["status"] = order.status;

                    j["parent_id"] = order.parent_id;

                    j["side"] = order.side;
                    j["offset"] = order.offset;
                    j["hedge_flag"] = order.hedge_flag;
                    j["price_type"] = order.price_type;
                    j["volume_condition"] = order.volume_condition;
                    j["time_condition"] = order.time_condition;

                }

                inline void from_json(const nlohmann::json &j, Order &order)
                {
                    order.order_id = j["order_id"];
                    order.insert_time = j["insert_time"];
                    order.update_time = j["update_time"];
                    order.set_trading_day(j["trading_day"].get<std::string>());
                    order.set_instrument_id(j["instrument_id"].get<std::string>());
                    order.set_exchange_id(j["exchange_id"].get<std::string>());
                    order.set_source_id(j["source_id"].get<std::string>());
                    order.set_account_id(j["account_id"].get<std::string>());
                    order.set_client_id(j["client_id"].get<std::string>());
                    order.instrument_type = j["instrument_type"];
                    order.limit_price = j["limit_price"].get<double>();
                    order.frozen_price = j["frozen_price"].get<double>();
                    order.volume = j["volume"];
                    order.volume_traded = j["volume_traded"];
                    order.volume_left = j["volume_left"];
                    order.tax = j["tax"].get<double>();
                    order.commission = j["commission"].get<double>();
                    order.error_id = j["error_id"];
                    order.set_error_msg(j["error_msg"].get<std::string>());
                    order.status = j["status"];
                    order.parent_id = j["parent_id"];
                    order.side = j["side"];
                    order.offset = j["offset"];
                    order.hedge_flag = j["hedge_flag"];
                    order.price_type = j["price_type"];
                    order.volume_condition = j["volume_condition"];
                    order.time_condition = j["time_condition"];
                }

                inline void order_from_input(const msg::data::OrderInput &input, msg::data::Order &order)
                {
                    order.parent_id = input.parent_id;
                    order.order_id = input.order_id;
                    strcpy(order.instrument_id, input.instrument_id);
                    strcpy(order.exchange_id, input.exchange_id);
                    strcpy(order.source_id, input.source_id);
                    strcpy(order.account_id, input.account_id);

                    order.limit_price = input.limit_price;
                    order.frozen_price = input.frozen_price;

                    order.volume = input.volume;
                    order.volume_traded = 0;
                    order.volume_left = input.volume;
                    order.status = OrderStatus::Submitted;

                    order.side = input.side;
                    order.offset = input.offset;
                    order.hedge_flag = input.hedge_flag;

                    order.price_type = input.price_type;
                    order.volume_condition = input.volume_condition;
                    order.time_condition = input.time_condition;
                }

                //????????????
                struct Trade
                {
                    uint64_t trade_id;                             //??????ID

                    uint64_t order_id;                       //??????ID
                    uint64_t parent_order_id;                //?????????ID

                    int64_t trade_time;                     //????????????
                    char trading_day[DATE_LEN];              //?????????

                    char instrument_id[INSTRUMENT_ID_LEN];   //??????ID
                    char exchange_id[EXCHANGE_ID_LEN];       //?????????ID
                    char source_id[SOURCE_ID_LEN];           //??????ID
                    char account_id[ACCOUNT_ID_LEN];         //??????ID
                    char client_id[CLIENT_ID_LEN];           //Client ID

                    InstrumentType instrument_type;          //????????????

                    Side side;                               //????????????
                    Offset offset;                           //????????????
                    HedgeFlag hedge_flag;                    //??????????????????

                    double price;                            //????????????
                    int64_t volume;                          //?????????

                    double tax;                              //???
                    double commission;                       //?????????

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    const std::string get_source_id() const
                    { return std::string(source_id); }

                    const std::string get_account_id() const
                    { return std::string(account_id); }

                    const std::string get_client_id() const
                    { return std::string(client_id); }

                    const std::string get_trading_day() const
                    {return std::string(trading_day);}

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN); }

                    void set_exchange_id(const std::string& exchange_id)
                    { strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN); }

                    void set_source_id(const std::string& source_id)
                    { strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN); }

                    void set_account_id(const std::string& account_id)
                    { strncpy(this->account_id, account_id.c_str(), ACCOUNT_ID_LEN); }

                    void set_client_id(const std::string& client_id)
                    { strncpy(this->client_id, client_id.c_str(), CLIENT_ID_LEN); }

                    void set_trading_day(const std::string& trading_day)
                    { strncpy(this->trading_day, trading_day.c_str(), DATE_LEN); }

#ifndef _WIN32
                } __attribute__((packed));
#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Trade &trade)
                {
                    j["trade_id"] = trade.trade_id;
                    j["order_id"] = std::to_string(trade.order_id);

                    j["trade_time"] = trade.trade_time;
                    j["trading_day"] = std::string(trade.trading_day);

                    j["instrument_id"] = std::string(trade.instrument_id);
                    j["exchange_id"] = std::string(trade.exchange_id);
                    j["account_id"] = std::string(trade.account_id);
                    j["client_id"] = std::string(trade.client_id);

                    j["instrument_type"] = trade.instrument_type;

                    j["side"] = trade.side;
                    j["offset"] = trade.offset;
                    j["hedge_flag"] = trade.hedge_flag;

                    j["price"] = trade.price;
                    j["volume"] = trade.volume;

                    j["tax"] = trade.tax;
                    j["commission"] = trade.commission;

                }

                inline void from_json(const nlohmann::json &j, Trade &trade)
                {
                    trade.trade_id = j["trade_id"];

                    trade.order_id = j["order_id"].get<uint64_t>();

                    trade.trade_time = j["trade_time"];
                    strncpy(trade.instrument_id, j["instrument_id"].get<std::string>().c_str(), INSTRUMENT_ID_LEN);
                    strncpy(trade.exchange_id, j["exchange_id"].get<std::string>().c_str(), EXCHANGE_ID_LEN);
                    strncpy(trade.account_id, j["account_id"].get<std::string>().c_str(), ACCOUNT_ID_LEN);
                    strncpy(trade.client_id, j["client_id"].get<std::string>().c_str(), CLIENT_ID_LEN);

                    trade.instrument_type = j["instrument_type"];

                    trade.side = j["side"];
                    trade.offset = j["offset"];
                    trade.hedge_flag = j["hedge_flag"];

                    trade.price = j["price"].get<double>();
                    trade.volume = j["volume"];

                    trade.tax = j["tax"].get<double>();
                    trade.commission = j["commission"].get<double>();
                }

                //????????????
                struct Asset
                {
                    int64_t update_time;               //????????????
                    char trading_day[DATE_LEN];        //?????????

                    uint32_t holder_uid;
                    LedgerCategory ledger_category;

                    char source_id[SOURCE_ID_LEN];     //??????ID
                    char broker_id[BROKER_ID_LEN];     //Broker ID
                    char account_id[ACCOUNT_ID_LEN];   //??????ID
                    char client_id[CLIENT_ID_LEN];     //client ID

                    double initial_equity;             //????????????
                    double static_equity;              //????????????
                    double dynamic_equity;             //????????????

                    double realized_pnl;               //????????????
                    double unrealized_pnl;

                    double avail;                      //????????????
                    double market_value;               //??????(??????)

                    double margin;                     //?????????(??????)

                    double accumulated_fee;            //???????????????
                    double intraday_fee;               //???????????????

                    double frozen_cash;                //????????????(??????: ??????????????????; ??????: ???????????????+???????????????)
                    double frozen_margin;              //???????????????(??????)
                    double frozen_fee;                 //???????????????(??????)

                    double position_pnl;               //????????????(??????)
                    double close_pnl;                  //????????????(??????)

                    const std::string get_trading_day() const
                    { return std::string(trading_day); }

                    const std::string get_account_id() const
                    { return std::string(account_id); }

                    const std::string get_client_id() const
                    { return std::string(client_id); }

                    const std::string get_source_id() const
                    { return std::string(source_id); }

                    void set_account_id(const std::string& account_id)
                    { strncpy(this->account_id, account_id.c_str(), ACCOUNT_ID_LEN); }

                    void set_client_id(const std::string& client_id)
                    { strncpy(this->client_id, client_id.c_str(), CLIENT_ID_LEN); }

                    void set_trading_day(const std::string& trading_day)
                    { strncpy(this->trading_day, trading_day.c_str(), DATE_LEN); }

                    void set_source_id(const std::string& source_id)
                    { strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN); }

#ifndef _WIN32
                } __attribute__((packed));

#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Asset &asset_info)
                {
                    j["update_time"] = asset_info.update_time;
                    j["holder_uid"] = asset_info.holder_uid;
                    j["ledger_category"] = asset_info.ledger_category;
                    j["trading_day"] = std::string(asset_info.trading_day);
                    j["account_id"] = std::string(asset_info.account_id);
                    j["source_id"] = std::string(asset_info.source_id);
                    j["client_id"] = std::string(asset_info.client_id);
                    j["initial_equity"] = asset_info.initial_equity;
                    j["static_equity"] = asset_info.static_equity;
                    j["dynamic_equity"] = asset_info.dynamic_equity;
                    j["unrealized_pnl"] = asset_info.unrealized_pnl;
                    j["realized_pnl"] =  asset_info.realized_pnl;
                    j["avail"] = asset_info.avail;
                    j["market_value"] = asset_info.market_value;
                    j["margin"] = asset_info.margin;
                    j["accumulated_fee"] = asset_info.accumulated_fee;
                    j["intraday_fee"] = asset_info.intraday_fee;
                    j["frozen_cash"] = asset_info.frozen_cash;
                    j["frozen_margin"] = asset_info.frozen_margin;
                    j["frozen_fee"] = asset_info.frozen_fee;
                    j["position_pnl"] = asset_info.position_pnl;
                    j["close_pnl"] = asset_info.close_pnl;
                }

                //????????????
                struct Position
                {
                    int64_t update_time;                     //????????????
                    char trading_day[DATE_LEN];              //?????????

                    char instrument_id[INSTRUMENT_ID_LEN];   //??????ID
                    InstrumentType instrument_type;          //????????????
                    char exchange_id[EXCHANGE_ID_LEN];       //?????????ID

                    uint32_t holder_uid;
                    LedgerCategory ledger_category;

                    char source_id[SOURCE_ID_LEN];           //??????ID
                    char account_id[ACCOUNT_ID_LEN];         //??????ID
                    char client_id[CLIENT_ID_LEN];           //Client ID

                    Direction direction;                     //????????????

                    int64_t volume;                          //??????
                    int64_t yesterday_volume;                //????????????
                    int64_t frozen_total;                    //????????????
                    int64_t frozen_yesterday;                //????????????

                    double last_price;                       //?????????

                    double avg_open_price;                   //????????????
                    double position_cost_price;              //???????????????

                    double close_price;                      //?????????(???????????????)
                    double pre_close_price;                  //?????????(???????????????)

                    double settlement_price;                 //?????????(??????)
                    double pre_settlement_price;             //?????????(??????)

                    double margin;                           //?????????(??????)
                    double position_pnl;                     //????????????(??????)
                    double close_pnl;                        //????????????(??????)

                    double realized_pnl;                     //???????????????
                    double unrealized_pnl;                   //???????????????

                    const std::string get_trading_day() const
                    { return std::string(trading_day); }

                    const std::string get_instrument_id() const
                    { return std::string(instrument_id); }

                    const std::string get_exchange_id() const
                    { return std::string(exchange_id); }

                    const std::string get_source_id() const
                    { return std::string(source_id); }

                    const std::string get_account_id() const
                    { return std::string(account_id); }

                    const std::string get_client_id() const
                    { return std::string(client_id); }

                    void set_instrument_id(const std::string& instrument_id)
                    { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN); }

                    void set_exchange_id(const std::string& exchange_id)
                    { strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN); }

                    void set_source_id(const std::string& source_id)
                    { strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN); }

                    void set_account_id(const std::string& account_id)
                    { strncpy(this->account_id, account_id.c_str(), ACCOUNT_ID_LEN); }

                    void set_client_id(const std::string& client_id)
                    { strncpy(this->client_id, client_id.c_str(), CLIENT_ID_LEN); }

                    void set_trading_day(const std::string& trading_day)
                    { strncpy(this->trading_day, trading_day.c_str(), DATE_LEN); }

#ifndef _WIN32
                } __attribute__((packed));

#else
                };
#endif

                inline void to_json(nlohmann::json &j, const Position &position)
                {
                    j["update_time"] = position.update_time;
                    j["trading_day"] = std::string(position.trading_day);
                    j["instrument_id"] = std::string(position.instrument_id);
                    j["instrument_type"] = position.instrument_type;
                    j["exchange_id"] = std::string(position.exchange_id);
                    j["account_id"] = std::string(position.account_id);
                    j["client_id"] = std::string(position.client_id);
                    j["direction"] = position.direction;
                    j["volume"] = position.volume;
                    j["yesterday_volume"] = position.yesterday_volume;
                    j["frozen_total"] = position.frozen_total;
                    j["frozen_yesterday"] = position.frozen_yesterday;
                    j["last_price"] = position.last_price;
                    j["avg_open_price"] = position.avg_open_price;
                    j["position_cost_price"] = position.position_cost_price;
                    j["close_price"] = position.close_price;
                    j["pre_close_price"] = position.pre_close_price;
                    j["settlement_price"] = position.settlement_price;
                    j["pre_settlement_price"] = position.pre_settlement_price;
                    j["margin"] = position.margin;
                    j["position_pnl"] = position.position_pnl;
                    j["close_pnl"] = position.close_pnl;
                    j["realized_pnl"] = position.realized_pnl;
                    j["unrealized_pnl"] = position.unrealized_pnl;
                }

            struct PositionEnd
            {
                uint32_t holder_uid;
#ifndef _WIN32
            } __attribute__((packed));
#else
            };
#endif

            //??????????????????
            struct PositionDetail
            {
                int64_t update_time;                     //????????????
                char trading_day[DATE_LEN];              //?????????

                char instrument_id[INSTRUMENT_ID_LEN];   //??????ID
                InstrumentType instrument_type;          //????????????
                char exchange_id[EXCHANGE_ID_LEN];       //?????????ID

                uint32_t holder_uid;

                char source_id[SOURCE_ID_LEN];           //??????ID
                char account_id[ACCOUNT_ID_LEN];         //??????ID
                char client_id[CLIENT_ID_LEN];           //Client ID

                Direction direction;                     //????????????

                int64_t volume;                          //??????
                int64_t frozen_volume;                   //????????????

                double last_price;                       //?????????
                double open_price;                       //?????????

                double settlement_price;                 //?????????(??????)
                double pre_settlement_price;             //?????????(??????)

                char open_date[DATE_LEN];                //?????????(YYYYMMDD,????????????????????????)

                uint64_t trade_id;                       //??????ID
                int64_t trade_time;                      //????????????

                const std::string get_trading_day() const
                { return std::string(trading_day); }

                const std::string get_instrument_id() const
                { return std::string(instrument_id); }

                const std::string get_exchange_id() const
                { return std::string(exchange_id); }

                const std::string get_source_id() const
                { return std::string(source_id); }

                const std::string get_account_id() const
                { return std::string(account_id); }

                const std::string get_client_id() const
                { return std::string(client_id); }

                const std::string get_open_date() const
                { return std::string(open_date); }

                void set_account_id(const std::string& account_id)
                { strncpy(this->account_id, account_id.c_str(), ACCOUNT_ID_LEN); }

                void set_client_id(const std::string& client_id)
                { strncpy(this->client_id, client_id.c_str(), CLIENT_ID_LEN); }

                void set_trading_day(const std::string& trading_day)
                { strncpy(this->trading_day, trading_day.c_str(), DATE_LEN); }

                void set_instrument_id(const std::string& instrument_id)
                { strncpy(this->instrument_id, instrument_id.c_str(), INSTRUMENT_ID_LEN); }

                void set_exchange_id(const std::string& exchange_id)
                { strncpy(this->exchange_id, exchange_id.c_str(), EXCHANGE_ID_LEN); }

                void set_source_id(const std::string& source_id)
                { strncpy(this->source_id, source_id.c_str(), SOURCE_ID_LEN); }

                void set_open_date(const std::string& open_date)
                { strncpy(this->open_date, open_date.c_str(), DATE_LEN); }

#ifndef _WIN32
            } __attribute__((packed));

#else
            };
#endif

            struct PositionDetailEnd
            {
                uint32_t holder_uid;

#ifndef _WIN32
            } __attribute__((packed));

#else
            };
#endif

#ifdef _WIN32
    #pragma pack(pop)
#endif

            inline void to_json(nlohmann::json &j, const PositionDetail &detail)
            {
                j["update_time"] = detail.update_time;
                j["trading_day"] = std::string(detail.trading_day);
                j["instrument_id"] = std::string(detail.instrument_id);
                j["instrument_type"] = detail.instrument_type;
                j["exchange_id"] = std::string(detail.exchange_id);
                j["account_id"] = std::string(detail.account_id);
                j["client_id"] = std::string(detail.client_id);
                j["direction"] = detail.direction;
                j["volume"] = detail.volume;
                j["frozen_volume"] = detail.frozen_volume;
                j["last_price"] = detail.last_price;
                j["open_price"] = detail.open_price;
                j["settlement_price"] = detail.settlement_price;
                j["pre_settlement_price"] = detail.pre_settlement_price;
                j["open_date"] = std::string(detail.open_date);

                j["trade_id"] = detail.trade_id;
                j["trade_time"] = detail.trade_time;

            }

            template<typename T> std::string to_string(const T &ori)
            {
                nlohmann::json j;
                to_json(j, ori);
                return j.dump(-1, ' ', false, nlohmann::json::error_handler_t::ignore);;
            }
            }
        }
    }
}

#endif //WINGCHUN_EVENT_H
