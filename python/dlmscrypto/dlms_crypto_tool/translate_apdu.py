from dlms_crypto_tool.xdlms_tag import COMMAND, GET
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

    elif COMMAND.get(apdu[0:2],"Command not supported!") == "SET_REQUEST_TAG":

        print("")

    else:

        print("")