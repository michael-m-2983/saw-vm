with open("code.saw", "wb") as fp:
    fp.write(b'saw')

    # version info
    fp.write(bytes([0, 2]))
    

    #TODO: instructions