# -*- coding: utf-8 -*-
'''
Copyright [2017] [taurus.ai]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
'''

import libwingchunstrategy
import datetime, time
from functools import partial

class dotdict(dict):
    """dot.notation access to dictionary attributes"""
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

context = dotdict()

def get_next_nano(time_str):
    cur_time = datetime.datetime.now()
    parse_time = datetime.datetime.strptime(time_str, "%H:%M:%S")
    new_time = cur_time.replace(hour=parse_time.hour, minute=parse_time.minute, second=parse_time.second)
    if new_time < cur_time:
        new_time += datetime.timedelta(days=1)
    return long(time.mktime(new_time.timetuple()) * 1e9)

help_art = '''\
.......................................................................
.   o   \ o /  _ o        __|    \ /     |__         o _  \ o /   o   .
.  /|\    |     /\   __\o   \o    |    o/     o/__   /\     |    /|\  .
.  / \   / \   | \  /) |    ( \  /o\  / )    |   (\  / |   / \   / \  .
.......................................................................
'''

context_usage = \
    [   # utilities
        ('stop', lambda strategy: strategy.get_data_wrapper().stop,
         'stop strategy reading process (not immediately, may wait several seconds before really stopped)',
         (None, [])),
        ('new_pos', lambda strategy: libwingchunstrategy.create_pos_handler,
         'create a new & empty position handler',
         ('PosHandler', [('int', 'source')])),
        ('get_nano', lambda strategy: strategy.get_strategy_util().get_nano,
         'get current nanoseconds',
         ('long', [])),
        ('get_effective_orders', lambda strategy: strategy.get_effective_orders,
         'get effective order ids (effective means still on the order book)',
         ('list', [])),
        ('get_name', lambda strategy: strategy.get_name,
         'get current strategy name',
         ('str', [])),
        ('parse_time', lambda strategy: strategy.get_strategy_util().parse_time,
         'parse format string ("%Y-%m-%d %H:%M:%S") into nano',
         ('long', [('str', 'time_str eg: \"2017-01-01 14:39:20\"')])),
        ('parse_nano', lambda strategy: lambda x: strategy.get_strategy_util().parse_nano(long(x)),
         'parse nano into format string ("%Y-%m-%d %H:%M:%S")',
         ('str', [('long', 'nano time')])),
        ('print_pos', lambda strategy: lambda x: print_pos(x),
         'print out position handler\'s holdings',
         (None, [('PosHandler', 'pos_handler')])),
        # setup
        ('add_td', lambda strategy: strategy.get_data_wrapper().add_trade_engine,
         'add trade engine, will get initial position in \"on_pos\" with request_id=-1',
         (None, [('int', 'source')])),
        ('add_md', lambda strategy: strategy.get_data_wrapper().add_market_data,
         'add market data engine, will get subscribed tickers in \"on_tick\" function',
         (None, [('int', 'source')])),
        # td requests
        ('req_pos', lambda strategy: strategy.req_position,
         'request for holding position, return request_id (-1 if failed), will get account\'s position in \"on_pos\" with request_id',
         ('int', [('int', 'source')])),
        ('cancel_order', lambda strategy: strategy.cancel_order,
         'cancel order by order_id, return request_id (-1 if failed)',
         ('int', [('int', 'source'), ('int', 'order_id')])),
        # position
        ('set_pos', lambda strategy: strategy.get_data_wrapper().set_pos,
         'ATTENTION! set internal holding position and have position in trade engine refreshed',
         (None, [('PosHandler', 'pos_handler'), ('int', 'source')])),
        ('get_pos', lambda strategy: strategy.get_data_wrapper().get_pos,
         'get internal position',
         ('PosHandler', [('int', 'source')])),
        # pnl
        ('get_ticker_pnl', lambda strategy: strategy.get_data_wrapper().get_ticker_pnl,
         'return ticker\'s pnl, position value minus trade cost and fees',
         (None, [('int', 'source'), ('str', 'ticker'), ('bool', 'include_fee')])),
        # market data
        ('subscribe', lambda strategy: strategy.get_strategy_util().subscribe_market_data,
         'subscribe market data. ATTENTION: tickers is list of string, eg: \"IF1712\", \"600000@SSZ\"',
         (None, [('list', 'tickers'), ('int', 'source')])),
        ('register_bar', lambda strategy: strategy.get_data_wrapper().register_bar_md,
         'register bar market data with specified interval',
         (None, [('int', 'source'), ('int', 'min_interval'), ('str', 'start_time eg:\"09:30:00\"'), ('str', 'end_time eg:\"11:30:00\"')])),
        ('disable_auto_sub', lambda strategy: strategy.get_data_wrapper().disable_auto_sub_mode,
         'disable auto subscription of existing holdings',
         (None, [])),
        # callback
        ('insert_func_at', lambda strategy: lambda x, y: strategy.get_strategy_util().insert_callback(long(x), y),
         'insert callback function with nano time',
         (None, [('long', 'nano'), ('func', 'function')])),
        ('insert_func_after', lambda strategy: lambda x, y: strategy.get_strategy_util().insert_callback(long(strategy.get_strategy_util().get_nano() + x * 1e9), y),
         'insert callback function after seconds',
         (None, [('int', 'seconds'), ('func', 'function')])),
        ('insert_func_at_c', lambda strategy: lambda x, y: strategy.get_strategy_util().insert_callback(long(x), partial(y, context)),
         'insert callback function with nano time with context as the first parameter',
         (None, [('long', 'nano'), ('func', 'function')])),
        ('insert_func_after_c', lambda strategy: lambda x, y: strategy.get_strategy_util().insert_callback(long(strategy.get_strategy_util().get_nano() + x * 1e9), partial(y, context)),
         'insert callback function after seconds with context as the first parameter',
         (None, [('int', 'seconds'), ('func', 'function')])),
        ('insert_func_at_next', lambda strategy: lambda x, y: strategy.get_strategy_util().insert_callback(get_next_nano(x), y),
         'insert callback function, will call back at next effective time.\neg: if call at \"09:30:00\" when 9:00, will callback today; will callback at tomorrow 9:30 if it is 10:00 now',
         (None, [('str', 'time_str'), ('func', 'function')])),
        ('insert_func_at_next_c', lambda strategy: lambda x, y: strategy.get_strategy_util().insert_callback(get_next_nano(x), partial(y, context)),
         'insert callback function with context as the first parameter, time part please refer \"insert_func_at_next\"',
         (None, [('str', 'time_str'), ('func', 'function')])),
        # insert order
        ('insert_market_order', lambda strategy: strategy.insert_market_order,
         'insert market order, return order_id (-1 if failed)',
         ('int', [('int', 'source'), ('str', 'ticker'), ('str', 'exchange_id'), ('int', 'volume'), ('DIRECTION', 'direction'), ('OFFSET ', 'offset')])),
        ('insert_limit_order', lambda strategy: strategy.insert_limit_order,
         'insert limit order, return order_id (-1 if failed)',
         ('int', [('int', 'source'), ('str', 'ticker'), ('str', 'exchange_id'), ('float', 'price'), ('int', 'volume'), ('DIRECTION', 'direction'), ('OFFSET', 'offset')])),
        ('insert_fok_order', lambda strategy: strategy.insert_fok_order,
         'insert fill-or-kill order, return order_id (-1 if failed)',
         ('int', [('int', 'source'), ('str', 'ticker'), ('str', 'exchange_id'), ('float', 'price'), ('int', 'volume'), ('DIRECTION', 'direction'), ('OFFSET', 'offset')])),
        ('insert_fak_order', lambda strategy: strategy.insert_fak_order,
         'insert fill-and-kill order, return order_id (-1 if failed)',
         ('int', [('int', 'source'), ('str', 'ticker'), ('str', 'exchange_id'), ('float', 'price'), ('int', 'volume'), ('DIRECTION', 'direction'), ('OFFSET', 'offset')])),
        # log
        ('log_debug', lambda strategy: lambda x: strategy.log_debug(str(x)),
         'print in log & console with DEBUG tag',
         (None, [('str', 'msg')])),
        ('log_info', lambda strategy: lambda x: strategy.log_info(str(x)),
         'print in log & console with INFO tag',
         (None, [('str', 'msg')])),
        ('log_error', lambda strategy: lambda x: strategy.log_error(str(x)),
         'print in log & console with ERROR tag',
         (None, [('str', 'msg')])),
        ('log_fatal', lambda strategy: lambda x: strategy.log_fatal(str(x)),
         'print in log & console with FATAL tag',
         (None, [('str', 'msg')])),
    ]

CONTEXT_COMMENT = 'wrap system-defined functions and internal data'
SOURCE_COMMENT = 'idx of source'

override_methods = [
    ('initialize', (None, [('context', 'context object', CONTEXT_COMMENT)]),
     'setup context, add market data / trade engine, do necessary initialization and subscription', True),
    ('on_bar', (None, [('context', 'context object', CONTEXT_COMMENT),
                       ('bars', 'dict', 'ticker(str) -> LFBarMarketDataField(bar structure)'),
                       ('min_interval', 'int', 'minute interval. eg: this is 5min bar if min_interval=5'),
                       ('source', 'int', SOURCE_COMMENT),
                       ('rcv_time', 'long', 'time of bar generated.')]),
     'callback when bar data received', False),
    ('on_tick', (None, [('context', 'context object', CONTEXT_COMMENT),
                        ('market_data', 'LFMarketDataField', 'market tick data'),
                        ('source', 'int', SOURCE_COMMENT),
                        ('rcv_time', 'long', 'time of tick received.')]),
     'callback when tick data received', False),
    ('on_pos', (None, [('context', 'context object', CONTEXT_COMMENT),
                       ('pos_handler', 'PosHandler', 'class of position getting and setting'),
                       ('request_id', 'int', 'request_id of req_pos, -1 if is initial callback when login td'),
                       ('source', 'int', SOURCE_COMMENT),
                       ('rcv_time', 'long', 'time of position information received.')]),
     'callback when (1) login td, td may return initial position with request_id=-1' \
     ' (2) context.req_pos is called \n(ATTENTION! here is the account\'s position instead of single strategy)', False),
    ('on_error', (None, [('context', 'context object', CONTEXT_COMMENT),
                         ('error_id', 'int', 'identity of error faced'),
                         ('error_msg', 'str', 'message of error faced'),
                         ('request_id', 'int', 'id of request, will be order_id if order inserted failed.'),
                         ('source', 'int', SOURCE_COMMENT),
                         ('rcv_time', 'long', 'time of error received.')]),
     'callback when request faces error', False),
    ('on_rtn_order', (None, [('context', 'context object', CONTEXT_COMMENT),
                             ('rtn_order', 'LFRtnOrderField', 'return-order data'),
                             ('order_id', 'int', 'request id of order'),
                             ('source', 'int', SOURCE_COMMENT),
                             ('rcv_time', 'long', 'time of rtn_order received.')]),
     'callback when updates of any order requested by current strategy is received', False),
    ('on_rtn_trade', (None, [('context', 'context object', CONTEXT_COMMENT),
                             ('rtn_trade', 'LFRtnTradeField', 'return-trade data'),
                             ('order_id', 'int', 'request id of order'),
                             ('source', 'int', SOURCE_COMMENT),
                             ('rcv_time', 'long', 'time of rtn_trade received.')]),
     'callback when trades of any order requested by current strategy is received', False),
    ('on_switch_day', (None, [('context', 'context object', CONTEXT_COMMENT),
                              ('rcv_time', 'long', 'time of switch_day signal received.')]),
     'callback when a trading day is switched', False),
]
class_details = [
    ('PosHandler', [], [('add_pos', (None, [('str', 'ticker'),
                                            ('POS_DIRECTION', 'direction'),
                                            ('int', 'tot_pos'),
                                            ('int', 'yd_pos'),
                                            ('float', 'balance'),
                                            ('float', 'fee')]),
                         'add position number of some ticker to existing holdings'),
                        ('set_pos', (None, [('str', 'ticker'),
                                            ('POS_DIRECTION', 'direction'),
                                            ('int', 'tot_pos'),
                                            ('int', 'yd_pos'),
                                            ('float', 'balance'),
                                            ('float', 'fee')]),
                         'set position number of some ticker'),
                        ('dump', ('str', []), 'dump to string'),
                        ('load', (None, [('str', 'json_str')]), 'load from string'),
                        ('get_tickers', ('list', []), 'get ticker names of holding positions, list of string'),
                        ('get_net_tot', ('int', [('str', 'ticker', 'ticker name')]), 'get total position of ticker with NET direction'),
                        ('get_net_yd', ('int', [('str', 'ticker')]), 'get yesterday position of ticker with NET direction'),
                        ('get_long_tot', ('int', [('str', 'ticker')]), 'get total position of ticker with LONG direction'),
                        ('get_long_yd', ('int', [('str', 'ticker')]), 'get yesterday position of ticker with LONG direction'),
                        ('get_short_tot', ('int', [('str', 'ticker')]), 'get total position of ticker with SHORT direction'),
                        ('get_short_yd', ('int', [('str', 'ticker')]), 'get yesterday position of ticker with SHORT direction'),
                        ('get_net_balance', ('float', [('str', 'ticker')]), 'get total trade amount of the ticker\'s NET position'),
                        ('get_net_fee', ('float', [('str', 'ticker')]), 'get total transaction fee of the ticker\'s NET position'),
                        ('get_long_balance', ('float', [('str', 'ticker')]), 'get total trade amount of the ticker\'s LONG position'),
                        ('get_long_fee', ('float', [('str', 'ticker')]), 'get total transaction fee of the ticker\'s LONG position'),
                        ('get_short_balance', ('float', [('str', 'ticker')]), 'get total trade amount of the ticker\'s SHORT position'),
                        ('get_short_fee', ('float', [('str', 'ticker')]), 'get total transaction fee of the ticker\'s SHORT position'),
                        ],
     'wrapper of position information, both internal strategy position and requested account position'),
    ('LFMarketDataField', [('TradingDay', 'str', '?????????'),
                           ('InstrumentID', 'str', '????????????'),
                           ('ExchangeID', 'str', '???????????????'),
                           ('ExchangeInstID', 'str', '???????????????????????????'),
                           ('LastPrice', 'float', '?????????'),
                           ('PreSettlementPrice', 'float', '???????????????'),
                           ('PreClosePrice', 'float', '?????????'),
                           ('PreOpenInterest', 'float', '????????????'),
                           ('OpenPrice', 'float', '?????????'),
                           ('HighestPrice', 'float', '?????????'),
                           ('LowestPrice', 'float', '?????????'),
                           ('Volume', 'int', '??????'),
                           ('Turnover', 'float', '????????????'),
                           ('OpenInterest', 'float', '?????????'),
                           ('ClosePrice', 'float', '?????????'),
                           ('SettlementPrice', 'float', '???????????????'),
                           ('UpperLimitPrice', 'float', '????????????'),
                           ('LowerLimitPrice', 'float', '????????????'),
                           ('PreDelta', 'float', '????????????'),
                           ('CurrDelta', 'float', '????????????'),
                           ('UpdateTime', 'str', '??????????????????'),
                           ('UpdateMillisec', 'int', '??????????????????'),
                           ('BidPrice1', 'float', '????????????'),
                           ('BidVolume1', 'int', '????????????'),
                           ('AskPrice1', 'float', '????????????'),
                           ('AskVolume1', 'int', '????????????'),
                           ('BidPrice2', 'float', '????????????'),
                           ('BidVolume2', 'int', '????????????'),
                           ('AskPrice2', 'float', '????????????'),
                           ('AskVolume2', 'int', '????????????'),
                           ('BidPrice3', 'float', '????????????'),
                           ('BidVolume3', 'int', '????????????'),
                           ('AskPrice3', 'float', '????????????'),
                           ('AskVolume3', 'int', '????????????'),
                           ('BidPrice4', 'float', '????????????'),
                           ('BidVolume4', 'int', '????????????'),
                           ('AskPrice4', 'float', '????????????'),
                           ('AskVolume4', 'int', '????????????'),
                           ('BidPrice5', 'float', '????????????'),
                           ('BidVolume5', 'int', '????????????'),
                           ('AskPrice5', 'float', '????????????'),
                           ('AskVolume5', 'int', '????????????')], [],
     'market data with details'),
    ('LFBarMarketDataField', [('TradingDay', 'str', '?????????'),
                              ('InstrumentID', 'str', '????????????'),
                              ('UpperLimitPrice', 'float', '????????????'),
                              ('LowerLimitPrice', 'float', '????????????'),
                              ('StartUpdateTime', 'str', '???tick????????????'),
                              ('StartUpdateMillisec', 'int', '???tick??????????????????'),
                              ('EndUpdateTime', 'str', '???tick??????????????????'),
                              ('EndUpdateMillisec', 'int', '???tick??????????????????'),
                              ('Open', 'float', '???'),
                              ('Close', 'float', '???'),
                              ('Low', 'float', '???'),
                              ('High', 'float', '???'),
                              ('Volume', 'float', '???????????????'),
                              ('StartVolume', 'float', '??????????????????')], [],
     'bar market data'),
    ('LFRtnOrderField', [('BrokerID', 'str', '??????????????????'),
                         ('ParticipantID', 'str', '????????????'),
                         ('InvestorID', 'str', '???????????????'),
                         ('BusinessUnit', 'str', '????????????'),
                         ('InstrumentID', 'str', '????????????'),
                         ('OrderRef', 'str', '????????????'),
                         ('ExchangeID', 'str', '???????????????'),
                         ('LimitPrice', 'float', '??????'),
                         ('VolumeTraded', 'int', '???????????????'),
                         ('VolumeTotal', 'int', '????????????'),
                         ('VolumeTotalOriginal', 'int', '??????'),
                         ('TimeCondition', 'TIME_CONDITION', '???????????????'),
                         ('VolumeCondition', 'VOLUME_TYPE', '???????????????'),
                         ('OrderPriceType', 'PRICE_TYPE', '??????????????????'),
                         ('Direction', 'DIRECTION', '????????????'),
                         ('OffsetFlag', 'OFFSET', '????????????'),
                         ('OrderStatus', 'ORDER_STATUS', '????????????')], [],
     'return order with order status'),
    ('LFRtnTradeField', [('BrokerID', 'str', '??????????????????'),
                         ('InvestorID', 'str', '???????????????'),
                         ('BusinessUnit', 'str', '????????????'),
                         ('InstrumentID', 'str', '????????????'),
                         ('OrderRef', 'str', '????????????'),
                         ('ExchangeID', 'str', '???????????????'),
                         ('Price', 'float', '??????'),
                         ('Volume', 'int', '??????'),
                         ('TradingDay', 'str', '?????????'),
                         ('TradeTime', 'str', '????????????'),
                         ('Direction', 'DIRECTION', '????????????'),
                         ('OffsetFlag', 'OFFSET', '????????????')], [],
     'return trade')
]

wingchun_constants = [
    ('SOURCE', 'int', [('CTP', 1, '')
                       ],
     'source index'),
    ('DIRECTION', 'char', [('Buy', '0', '???'),
                           ('Sell', '1', '???')],
     'order direction type'),
    ('OFFSET', 'char', [('Open', '0', '??????'),
                        ('Close', '1', '??????'),
                        ('CloseToday', '3', '??????'),
                        ('CloseYesterday', '4', '??????')],
     'order offset flag type'),
    ('PRICE_TYPE', 'char', [('AnyPrice', '1', '?????????'),
                            ('LimitPrice', '2', '??????'),
                            ('BestPrice', '3', '?????????')],
     'order price type (return-order / return-trade)'),
    ('ORDER_STATUS', 'char', [('AllTraded', '0', '??????????????????????????????'),
                              ('PartTradedQueueing', '1', '???????????????????????????'),
                              ('PartTradedNotQueueing', '2', '????????????????????????????????????????????? ???????????????'),
                              ('NoTradeQueueing', '3', '????????????????????????'),
                              ('NoTradeNotQueueing', '4', '??????????????????????????????????????????????????????'),
                              ('Canceled', '5', '??????'),
                              ('Unknown', 'a', '??????'),
                              ],
     'order status type (return-order)'),
    ('POS_DIRECTION', 'char', [('Net', '1', '???'),
                               ('Long', '2', '??????'),
                               ('Short', '3', '??????'),
                               ],
     'position direction type (PosHandler)'),
    ('VOLUME_TYPE', 'char', [('AV', '1', '????????????'),
                             ('MV', '2', '????????????'),
                             ('CV', '3', '????????????'),
                             ],
     'order volume condition type (return-order / return-trade)'),
    ('TIME_CONDITION', 'char', [('IOC', '1', '???????????????????????????'),
                                ('GFS', '2', '????????????'),
                                ('GFD', '3', '????????????'),
                                ('GTD', '4', '?????????????????????'),
                                ('GTC', '5', '???????????????'),
                                ('GFA', '6', '??????????????????'),
                                ],
     'order time condition type (return-order / return-trade)'),
    ('EXCHANGE', 'str',[('SSE', 'SSE', '?????????????????????'),
                        ('SZE', 'SZE', '?????????????????????'),
                        ('CFFEX', 'CFFEX', '???????????????????????????'),
                        ('SHFE', 'SHFE', '?????????????????????'),
                        ('DCE', 'DCE', '?????????????????????'),
                        ('CZCE', 'CZCE', '?????????????????????'),
                        ],
     'exchange short name')
]

def help_constant():
    print  help_art, '''
wingchun constants:

eg: insert a buy order, you may set direction as DIRECTION.Buy or \"0\" directly
        '''
    name_length = max(map(lambda x: len(x[0]), wingchun_constants)) + 2
    for struct_name, type_name, constants, comment in wingchun_constants:
        struct_name = '[\033[1;31;40m{}\033[0m]'.format(struct_name).ljust(name_length + 14)
        type_name_ = '[\033[1;35;40m{}\033[0m]'.format(type_name).ljust(20)
        print '{} {} {}'.format(struct_name, type_name_, comment)
        for constant_name, value, c_comment in constants:
            constant_name_length = max(map(lambda x: len(str(x[0])), constants)) + 2
            constant_name = '\033[4;36;40m{}\033[0m'.format(constant_name).ljust(constant_name_length + 14)
            value_length = max(map(lambda x: len(str(x[1])), constants)) + 2
            if type_name == 'char' or type_name == 'str':
                value = '\'\033[1;33;40m{}\033[0m\''.format(value).ljust(value_length + 14)
            else:
                value = '\033[1;33;40m{}\033[0m'.format(value).ljust(value_length + 14)
            print '{} {}{} {}'.format(' ' * (name_length + 7), constant_name, value, c_comment)

def help_class():
    print  help_art, '''
wingchun classes:
        '''
    name_length = max(map(lambda x: len(x[0]), class_details)) + 2
    output_length = max(map(lambda x: len(x[2][1]) if len(x[2]) > 1 else 0 , class_details))
    for class_name, fields, functions, comment in class_details:
        class_name = '[\033[1;31;40m{}\033[0m]'.format(class_name).ljust(name_length + 14)
        print '{} {}'.format(class_name, comment)
        for func_name, in_outs, func_comment in functions:
            func_name_length = max(map(lambda x: len(x[0]), functions)) + 2
            func_name = '[\033[1;35;40m{}\033[0m]'.format(func_name).ljust(func_name_length + 14)
            print '{} {}{}'.format(' ' * name_length, func_name, func_comment)
            output_str = 'None' if in_outs[0] is None else in_outs[0]
            input_str = ', '.join(map(lambda x: '{}[{}]'.format(x[1], x[0]), in_outs[1]))
            param_str = '\033[4;33;40m{}\033[0m <- (\033[4;36;40m{}\033[0m)'.format(output_str.ljust(output_length), input_str)
            print '{} {}'.format(' ' * (name_length + func_name_length), param_str)
        for field_name, type_name, field_comment in fields:
            field_name_length = max(map(lambda x: len(x[0]), fields)) + 2
            field_name = '\033[4;36;40m{}\033[0m'.format(field_name).ljust(field_name_length + 14)
            type_name_length = max(map(lambda x: len(x[1]), fields)) + 2
            type_name = '[\033[4;33;40m{}\033[0m]'.format(type_name).ljust(type_name_length + 14)
            print '{} {}{} {}'.format(' ' * name_length, field_name, type_name, field_comment)


def help():
    print  help_art, '''
usage: 
    wingchun md ${gateway_name} // run market data engine on gateway_name (eg: ctp)
    wingchun td ${gateway_name} // run trade engine on gateway_name (eg: ctp)
    wingchun strategy -n ${strategy_name} -p ${py_file_path} // run strategy in py_file_path
    
    wingchun pos -t get/set -n ${strategy_name} -s ${source_index} // get/set strategy internal position
    wingchun report -n ${strategy_name} -s ${start_time} -e ${end_time} -a // report strategy latency
    
    wingchun help context  // context usage details
    wingchun help function // functions that can be override in py_file_path
    wingchun help class    // internal data structure usage.
    wingchun help constant // internal constants.
    
    wingchun help pos      // help "wingchun pos" usage
    wingchun help report   // help "wingchun report" usage
    '''

def help_context():
    print  help_art, '''
context methods:
        '''
    name_length = max(map(lambda x: len(x[0]), context_usage)) + 2
    output_length = max(map(lambda x: 0 if x[3][0] is None else len(x[3][0]), context_usage))
    for _func_name, transfer_func, comment, params in context_usage:
        func_name = '[\033[1;31;40m{}\033[0m]'.format(_func_name).ljust(name_length + 14)
        output_str = 'None' if params[0] is None else params[0]
        input_str = ', '.join(map(lambda x: '{}[{}]'.format(x[1], x[0]), params[1]))
        param_str = '\033[4;33;40m{}\033[0m <- (\033[4;36;40m{}\033[0m)'.format(output_str.ljust(output_length), input_str)
        cmts = comment.split('\n')
        print '{} {}'.format(func_name, cmts[0])
        for cmt in cmts[1:]:
            print '{} {}'.format(' '.ljust(name_length), cmt)
        print '{} {}'.format(' ' * (name_length), param_str)


def help_function():
    print  help_art, '''
override functions: functions with mark (*) are necessary.
        '''
    name_length = max(map(lambda x: len(x[0]), override_methods)) + 2
    for func_name, params, comment, must_be in override_methods:
        func_name = '[\033[1;31;40m{}\033[0m]'.format(func_name).ljust(name_length + 14)
        if must_be:
            comment = '\033[4;36;40m(*)\033[0m' + comment
        if '\n' in comment:
            comments = comment.split('\n')
            print '{} {}'.format(func_name, comments[0])
            for i in range(1, len(comments)):
                print ' ' * name_length, comments[i]
        else:
            print '{} {}'.format(func_name, comment)
        for param in params[1]:
            print ' ' * name_length, '\033[4;33;40m{} [{}]\033[0m'.format(param[0], param[1]).ljust(20), param[2]


def print_pos(pos_handler):
    for ticker in pos_handler.get_tickers():
        print '{}\t(net){},{}[{},{}] (long){},{}[{},{}] (short){},{}[{},{}]'.format(
            ticker, pos_handler.get_net_tot(ticker), pos_handler.get_net_yd(ticker),
            pos_handler.get_net_balance(ticker), pos_handler.get_net_fee(ticker),
            pos_handler.get_long_tot(ticker), pos_handler.get_long_yd(ticker),
            pos_handler.get_long_balance(ticker), pos_handler.get_long_fee(ticker),
            pos_handler.get_short_tot(ticker), pos_handler.get_short_yd(ticker),
            pos_handler.get_short_balance(ticker), pos_handler.get_short_fee(ticker)
        )
