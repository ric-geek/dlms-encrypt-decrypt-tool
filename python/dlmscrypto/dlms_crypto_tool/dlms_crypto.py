import os
from binascii import unhexlify
from cryptography.hazmat.primitives.ciphers.aead import AESGCM

# Define security headers for different operations
SECURITY_HEADER = {

    'auth': "10", # Used for authentication AAD header
    'data': "30"  # Used for encrypt/decrypt AAD header
}

def create_iv(system_title: str, frame_counter: str) -> bytes:
    """
    Create an IV by concatenating the system title and frame counter.
    Both inputs are expected as hex strings.

    :param system_title: Hex string representing the system title.
    :param frame_counter: Hex string representing the frame counter.
    :return: The IV as bytes.
    """