from xdlms_tag import *

def translate_apdu(apdu: str) -> list[str]:

    apdu_field = [] # It will contain the translated APDU

    if TAGS.get(apdu[0:2],"Command not supported!") == "GET_REQUEST_TAG":

        apdu_field.append(TAGS.get(apdu[0:2], "Command not supported!"))
        apdu_field.append(TAGS.get(apdu[2:4], "Command not supported!"))
        apdu_field.append(TAGS.get(apdu[4:6], "Command not supported!"))
        apdu_field.append(TAGS.get(apdu[6:10], "Command not supported!"))
        apdu_field.append(TAGS.get(apdu[10:22], "Command not supported!"))
        apdu_field.append(TAGS.get(apdu[22:24], "Command not supported!"))

    elif TAGS.get(apdu[0:2],"Command not supported!") == "SET_REQUEST_TAG":

        print("")

    else:

        print("")

    return apdu_field