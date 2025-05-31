from collections import namedtuple

# TAGS = {
#     "01": "ACCESS_SELECTION",
#     "01": "ACCESS_SELECTOR",
#     "c4": "GET_RESPONSE_TAG",
#     "01": "SET_REQUEST_NORMAL_TAG",
#     "02": "SET_REQUEST_WITH_FIRST_DATABLOCK_TAG",
#     "03": "SET_REQUEST_WITH_DATABLOCK_TAG",
#     "04": "SET_REQUEST_WITH_LIST_TAG",
#     "01": "ACTION_REQUEST_NORMAL_TAG",
#     "02": "ACTION_REQUEST_NEXT_TAG",
#     "03": "ACTION_REQUEST_WITH_LIST_TAG",
#     "c7": "ACTION_RESPONSE",
#     "01": "ACTION_RESPONSE_NORMAL",
#     "00": "SUCCESS",
#     "fa": "OTHER_REASON",
#     "0b": "OBJECT_UNAVAILABLE",
#     "02": "TEMPORARY_FAILURE",
#     "03": "READ_WRITE_DENIED",
#     "0c": "TYPE_UNMATCHED",
#     "0d": "SCOPE_OF_ACCESS_VIOLATED"
# }

COSEM_ATTRIBUTE_DESCRIPTOR = namedtuple('CosemAttributeDescriptor',
                                        ['ClassId','InstanceId','AttributeId'])

DATA = {
    "0": "null-data",
    "1": "Array",
    "2": "Structure",
    "3": "Boolean",
    "4": "BitString",
    "5": "DoubleLong",
    "6": "DoubleLongUnsigned",
    "9": "OctetString",
    "10": "VisibleString",
    "12": "UTF8String",
    "13": "BCD",
    "15": "Integer",
    "16": "Long",
    "17": "Unsigned",
    "18": "LongUnsigned",
    "19": "CompactArray",
    "20": "Long64",
    "21": "Long64Unsigned",
    "22": "Float32",
    "23": "Float64",
    "24": "DataTime",
    "25": "Date",
    "26": "Time",
    "27": "DontCare",
}

COMMAND = {
    "c0": "GetRequest",
    "c1": "SetRequest",
    "c3": "ActionRequest",
}

GET = {
    "01": "GetRequestNormal",
    "02": "GetRequestNext",
    "03": "GetRequestWithList"
}

GET_REQUEST_NORMAL = {
    "01": "GetRequestNormal",
    "02": "GetRequestNext",
    "03": "GetRequestWithList"
}

GET_RESPONSE = {
    "01": "GetResponseNormal",
    "02": "GetResponseWithDatablock",
    "03": "GetResponseWithList"
}

GET_RESPONSE_NORMAL = {
    "01": "GetRequestNormal",
    "02": "GetRequestNext",
}

SET = {
    "01": "SetRequestNormal",
    "02": "SetRequestWithFirstDatablock",
    "03": "SetRequestWithDatablock",
    "04": "SetRequestWithList",
    "05": "SetRequestWithListAndFirstDatablock"
}

SET_RESPONSE = {
    "01": "SetResponseNormal",
}

SET_RESPONSE_NORMAL = {
    "01": "SetResponseNormal", # TODO HOW TO HANDLE?
}

ACTION = {
    "01": "ActionRequestNormal",
    "02": "ActionRequestNextPblock",
    "03": "ActionRequestWithList",
    "04": "ActionRequestWithListFirstPblock",
    "05": "ActionRequestWithListAndFirstDatablock",
    "06": "ActionRequestWithPblock"
}