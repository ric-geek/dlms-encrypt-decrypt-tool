from collections import namedtuple


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

DATA_ACCESS_RESULT = {
    "0": "Success",
    "1": "Hardware Fault",
    "2": "Temporary Failure",
    "3": "Read Write Denied",
    "4": "Object Undefined",
    "9": "Object Class Inconsistent",
    "11": "Object Unavailable",
    "12": "Type Unmatched",
    "13": "Scope Of Access Violated",
    "15": "Data Block Unavailable",
    "16": "Long Get Aborted",
    "17": "Long Set Aborted",
    "18": "No Long Set In Progress",
    "19": "Data Block Number Invalid",
    "255": "Other Reason"
}

COMMAND = {
    "c0": "GetRequest",
    "c1": "SetRequest",
    "c3": "ActionRequest",
    "c4": "GetResponse",
    "c5": "SetResponse"
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

GET_RESPONSE_NORMAL = ["InvokeIdAndPriority", DATA]

SET = {
    "01": "SetRequestNormal",
    "02": "SetRequestWithFirstDatablock",
    "03": "SetRequestWithDatablock",
    "04": "SetRequestWithList",
    "05": "SetRequestWithListAndFirstDatablock"
}

SET_REQUEST_NORMAL = ["InvokeIdAndPriority", COSEM_ATTRIBUTE_DESCRIPTOR, DATA]

SET_RESPONSE = {
    "01": "SetResponseNormal",
    "02": "SetResponseDatablock",
    "03": "SetResponseLastDatablock",
    "04": "SetResponseLastDatablockWithList",
    "05": "SetResponseWithList"
}

SET_RESPONSE_NORMAL = ["InvokeIdAndPriority", DATA_ACCESS_RESULT]

ACTION = {
    "01": "ActionRequestNormal",
    "02": "ActionRequestNextPblock",
    "03": "ActionRequestWithList",
    "04": "ActionRequestWithListFirstPblock",
    "05": "ActionRequestWithListAndFirstDatablock",
    "06": "ActionRequestWithPblock"
}

ACTION_REQUEST_NORMAL = ["InvokeIdAndPriority", COSEM_ATTRIBUTE_DESCRIPTOR, DATA]