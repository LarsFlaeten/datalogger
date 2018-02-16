import klv
import json
import struct

def unpack_str(value):
    return ''.join(struct.unpack('s'*len(value), value))

def parse_blackbox(filename):
#    print "In parse_blackbox"
    bf = open(filename,'r') 
    try:
        ba = bytearray(bf.read())
    finally:
        bf.close

    black = {}  # our blackbox structure
    ptr = 0;
    step = 0;
    timesteps = []
    step_dvalues = []
    step_ivalues = []
    header = False
    iclasscount = 0
    registered_classes = []
    class_num_values = []
    class_values = []
    class_types  = []
    cli = 0
    vli = 0
    while ptr < len(ba):
        key, value = klv.decode(ba[ptr:], 1)
        length, ber_len = klv.decode_ber(ba[ptr+1:ptr+6])
        # Get key as uint8 
        key = struct.unpack('B',key)[0]
        if key == 34:
            #print "Header, key <{}>, length={}, contains:".format(key, length)
            #Parse header:
            iptr = 0
            while(iptr < len(value)):
                ikey, ivalue = klv.decode(value[iptr:], 1)
                ilength, iber_len = klv.decode_ber(value[iptr+1:iptr+6])
                ikey = struct.unpack('B',ikey)[0]
                #print "    Key <{}>, length={}".format(ikey, ilength)
                 
                if ikey == 35: # Header version
                    iversion = struct.unpack('B',ivalue)[0]
                    assert(iversion == 1)
                elif ikey == 36: # Class count
                    iclasscount = struct.unpack('B',ivalue)[0]
                    registered_classes = [None] * iclasscount
                    class_num_values = [None] * iclasscount
                    class_values = [None] * iclasscount
                    class_types = [None] * iclasscount
                    step_dvalues = [None] * iclasscount
                    step_ivalues = [None] * iclasscount
                elif ikey == 37: # Indexed class name
                    clindex = struct.unpack('B', ivalue[:1])[0]
                    clname = unpack_str(ivalue[1:])
                    registered_classes[clindex] = clname
                elif ikey == 38:
                    clindex = struct.unpack('B', ivalue[:1])[0]
                    clnv    = struct.unpack('B', ivalue[1:2])[0]
                    class_num_values[clindex] = clnv
                    class_values[clindex] = [None]*clnv
                    class_types[clindex] = [None]*clnv
                    step_dvalues[clindex] = [None]*clnv
                    step_ivalues[clindex] = [None]*clnv
                elif ikey == 39:
                    cli = struct.unpack('B', ivalue[:1])[0]
                    vli = struct.unpack('B', ivalue[1:2])[0]
                elif ikey == 40:
                    vname = unpack_str(ivalue)
                    class_values[cli][vli] = vname
                elif ikey == 41:
                    vtype = unpack_str(ivalue)
                    class_types[cli][vli] = vtype
                
                iptr += 1 + iber_len + len(ivalue)
            # Set up the blacbox dict:
            for i in range(iclasscount):
                s = registered_classes[i];
                black[s] = {}
                # 'x' is values, will be assigned later
                black[s]['x'] = []
                # 'legend' is our value names
                black[s]['legend'] = tuple(class_values[i])
                black[s]['unit'] = "unit"    # We need to implement units from datalogger
            
            header = True

        elif key == 42:
            # key 42: Start step, uint64_t
            step = struct.unpack('Q', value)[0]
            # reset dvalues and ivalues for this step
            for i in range(0,iclasscount):
                for j in range(0,class_num_values[i]):
                    step_ivalues[i][j] = None
                    step_dvalues[i][j] = None
        elif key == 43: 
            # Key 43: end step uint64_t
            endstep = struct.unpack('Q', value)[0]
            assert(endstep == step)
            
            # For each class, we can now make the step data
            # Here we assume all value in this class is of same type - this needs to be enforced in datalogger... TODO
            for i in range(0,iclasscount):
                if class_types[i][0] == "DOUBLE":
                    black[registered_classes[i]]['x'].append(tuple(step_dvalues[i]))
                elif class_types[i][0] == "INT":
                    black[registered_classes[i]]['x'].append(tuple(step_ivalues[i]))
                
        elif key == 44:
            # Get class and value index
            i = struct.unpack('B', value[0:1])[0]
            j = struct.unpack('B', value[1:2])[0]
            
            if  class_types[i][j]=="DOUBLE":
                step_dvalues[i][j] = struct.unpack('d', value[2:length])[0]
            elif class_types[i][j]=="INT":
                step_ivalues[i][j] = struct.unpack('i', value[2:length])[0]
            

        else:
            print "Unrecognized klv: <{}>, payload length: {} bytes".format(key, length)


        ptr += 1+ ber_len + len(value)

    # We then reorganize the time ('t') so that it mathces the expected layout
    time_us = []
    for tu in black["TIME"]['x']:
        time_us.append(tu[0])

    black['timescale'] = time_us
    #time_us = [40.1,50.2,60.3,70.4,80.5,90.6]
    #black = {}

    # 'ap_state' or whatever is a dict containing the following:
    # 'x' shall contains a list of tuples
    # 'legend' shall contain a tuple of strings
    # 'unit' is just a string
    #black['ap_state'] = {}
    #"x", "legend", "unit"}
    #black['ap_state']['x'] = [(1,),(1,),(2,),(2,),(1,),(4,)]
    #black['ap_state']['legend'] = ("state",)
    #black['ap_state']['unit'] = "apfsmstate"

    #black['timescale'] = time_us

    #print type(black['ap_state']['x'])

    return black

