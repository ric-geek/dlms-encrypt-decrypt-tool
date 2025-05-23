from xdlms_tag import *

def translate_apdu(apdu: str) -> str:

    for i in range(0, len(apdu), 2):

        apdu_element = apdu[i:i+2]


    return apdu