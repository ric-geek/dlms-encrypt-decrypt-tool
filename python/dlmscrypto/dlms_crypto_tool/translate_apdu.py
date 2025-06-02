from dlms_crypto_tool.xdlms_tag import *
from lxml import etree


def translate_apdu(apdu: str):

    root = etree.Element("DlmsApdu") # Root node of the XML

    if COMMAND.get(apdu[0:2].lower(),"Command not supported!") == "GetRequest":

        get_request = etree.SubElement(root, COMMAND.get(apdu[0:2],"Command not supported!"))
        get_request_normal = etree.SubElement(get_request, GET.get(apdu[2:4],"Command not supported!"))
        invoke_id_and_priority = etree.SubElement(get_request_normal, "InvokeIdAndPriority", {"Value": apdu[4:6]})
        attribute_descriptor = etree.SubElement(invoke_id_and_priority, "AttributeDescriptor")
        class_id = etree.SubElement(attribute_descriptor, "ClassId", {"Value": apdu[6:10]})
        istance_id = etree.SubElement(class_id, "IstanceId", {"Value": apdu[10:22]})
        etree.SubElement(istance_id, "AttributeId", {"Value": apdu[22:24]})

        print(etree.tostring(root, pretty_print=True).decode())

    elif COMMAND.get(apdu[0:2].lower(),"Command not supported!") == "GetResponse":

        get_response = etree.SubElement(root, COMMAND.get(apdu[0:2].lower(), "Command not supported!"))
        get_response_normal = etree.SubElement(get_response, GET_RESPONSE.get(apdu[2:4].lower(), "Command not supported!"))

        # Unpack the list
        id_and_priority, data_type = GET_RESPONSE_NORMAL
        invoke_id_and_priority = etree.SubElement(get_response_normal, id_and_priority, {"Value": apdu[4:6]})
        result = etree.SubElement(invoke_id_and_priority, "Result")
        data_tag = etree.SubElement(result, "Data")
        etree.SubElement(data_tag, data_type[str(int(apdu[8:10], 16))], {"Value": apdu[10:]})

        print(etree.tostring(root, pretty_print=True).decode())

    elif COMMAND.get(apdu[0:2].lower(),"Command not supported!") == "SetRequest":

        set_request = etree.SubElement(root, COMMAND.get(apdu[0:2], "Command not supported!"))
        set_request_normal = etree.SubElement(set_request, SET.get(apdu[2:4], "Command not supported!"))
        invoke_id_and_priority = etree.SubElement(set_request_normal, "InvokeIdAndPriority", {"Value": apdu[4:6]})
        attribute_descriptor = etree.SubElement(invoke_id_and_priority, "AttributeDescriptor")
        class_id = etree.SubElement(attribute_descriptor, "ClassId", {"Value": apdu[6:10]})
        istance_id = etree.SubElement(class_id, "IstanceId", {"Value": apdu[10:22]})
        etree.SubElement(istance_id, "AttributeId", {"Value": apdu[22:24]})

        print(etree.tostring(root, pretty_print=True).decode())

    elif COMMAND.get(apdu[0:2].lower(),"Command not supported!") == "SetResponse":

        set_response = etree.SubElement(root, COMMAND.get(apdu[0:2].lower(), "Command not supported!"))
        set_response_normal = etree.SubElement(set_response, SET_RESPONSE.get(apdu[2:4].lower(), "Command not supported!"))

        # Unpack the list
        id_and_priority, data_result = SET_RESPONSE_NORMAL
        invoke_id_and_priority = etree.SubElement(set_response_normal, id_and_priority, {"Value": apdu[4:6]})
        etree.SubElement(invoke_id_and_priority, "Result", {"Value": data_result[str(int(apdu[6:], 16))]})

        print(etree.tostring(root, pretty_print=True).decode())

    elif COMMAND.get(apdu[0:2].lower(),"Command not supported!") == "ActionRequest":

        action_request = etree.SubElement(root, COMMAND.get(apdu[0:2].lower(), "Command not supported!"))
        action_request_normal = etree.SubElement(action_request, ACTION.get(apdu[2:4], "Command not supported!"))

        # Unpack the list
        id_and_priority, cosem_attribute, data_result = ACTION_REQUEST_NORMAL
        etree.SubElement(action_request_normal, id_and_priority, {"Value": apdu[4:6]})
        method_descriptor = etree.SubElement(action_request_normal, "MethodDescriptor")

        cad = cosem_attribute(apdu[6:10], apdu[10:22], apdu[22:24])
        etree.SubElement(method_descriptor, "ClassId", {"Value": cad.ClassId})
        etree.SubElement(method_descriptor, "InstanceId", {"Value": cad.InstanceId})
        etree.SubElement(method_descriptor, "MethodId", {"Value": cad.AttributeId})

        print(etree.tostring(root, pretty_print=True).decode())

    else:

        print("Command not supported!")