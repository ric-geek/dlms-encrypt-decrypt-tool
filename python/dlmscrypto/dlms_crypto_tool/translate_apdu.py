from dlms_crypto_tool.xdlms_tag import *
from lxml import etree


def translate_apdu(apdu: str):

    root = etree.Element("root") # Root node of the XML

    if COMMAND.get(apdu[0:2],"Command not supported!") == "GetRequest":

        get_request = etree.SubElement(root, COMMAND.get(apdu[0:2],"Command not supported!"))
        get_request_normal = etree.SubElement(get_request, GET.get(apdu[2:4],"Command not supported!"))
        invoke_id_and_priority = etree.SubElement(get_request_normal, "InvokeIdAndPriority", {"Value": apdu[4:6]})
        attribute_descriptor = etree.SubElement(invoke_id_and_priority, "AttributeDescriptor")
        class_id = etree.SubElement(attribute_descriptor, "ClassId", {"Value": apdu[6:10]})
        istance_id = etree.SubElement(class_id, "IstanceId", {"Value": apdu[10:22]})
        etree.SubElement(istance_id, "AttributeId", {"Value": apdu[22:24]})

        print(etree.tostring(root, pretty_print=True).decode())

    elif COMMAND.get(apdu[0:2],"Command not supported!") == "SetRequest":

        print("")

    elif COMMAND.get(apdu[0:2],"Command not supported!") == "GetResponse":

        get_response = etree.SubElement(root, COMMAND.get(apdu[0:2], "Command not supported!"))
        get_response_normal = etree.SubElement(get_response, GET_RESPONSE.get(apdu[2:4], "Command not supported!"))

        # Unpack the list
        id_and_priority, data_type = GET_RESPONSE_NORMAL
        invoke_id_and_priority = etree.SubElement(get_response_normal, id_and_priority, {"Value": apdu[4:6]})
        result = etree.SubElement(invoke_id_and_priority, "Result")
        etree.SubElement(result, data_type[apdu[6:10]], {"Value": apdu[6:10]}) #TODO FIX APDU INDEX

        print(etree.tostring(root, pretty_print=True).decode())

    else:

        print("")