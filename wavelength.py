def wavelength_to_rgb(wavelength, gamma=0.8):
    print ("hello")
    print
    arr = {}
    i = 0
    temp = {}
    update = 2.5
    for wavelength in range(379,751):
        wavelength = float(wavelength)
        if wavelength >= 380 and wavelength <= 440:
            attenuation = 0.3 + 0.7 * (wavelength - 380) / (440 - 380)
            R = ((-(wavelength - 440) / (440 - 380)) * attenuation) ** gamma
            G = 0.0
            B = (1.0 * attenuation) ** gamma
        elif wavelength >= 440 and wavelength <= 490:
            R = 0.0
            G = ((wavelength - 440) / (490 - 440)) ** gamma
            B = 1.0
        elif wavelength >= 490 and wavelength <= 510:
            R = 0.0
            G = 1.0
            B = (-(wavelength - 510) / (510 - 490)) ** gamma
        elif wavelength >= 510 and wavelength <= 580:
            R = ((wavelength - 510) / (580 - 510)) ** gamma
            G = 1.0
            B = 0.0
        elif wavelength >= 580 and wavelength <= 645:
            R = 1.0
            G = (-(wavelength - 645) / (645 - 580)) ** gamma
            B = 0.0
        elif wavelength >= 645 and wavelength <= 750:
            attenuation = 0.3 + 0.7 * (750 - wavelength) / (750 - 645)
            R = (1.0 * attenuation) ** gamma
            G = 0.0
            B = 0.0
        else:
            R = 0.0
            G = 0.0
            B = 0.0
        R *= 255
        G *= 255
        B *= 255
        R = int(R)
        G = int(G)
        B = int(B)
        triplet = (R, G, B)
        #print(wavelength,'=' ,R, G, B)
        arr[i] = '#'+''.join(map(chr, triplet)).encode('hex')+' ' + str(update)+'%, '
        temp = str(temp) + arr[i]
        #arr[1] = '#'+''.join(map(chr, triplet)).encode('hex')+' ' +'0%'
        i= i +1
        update = update+0.256
    
    for j in range (0,i):
       print (arr[j] + ',')
    print (temp)
# minimum =380 maximum = 750  
wavelength_to_rgb(1)
