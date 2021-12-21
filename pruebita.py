example ="30:65:EC:6F:C4:AF"
example_2 = "30:65:EC:6F:C:AF"

validar= ["1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"]

def fun(mac_add, validar):
    cont = 0
    cont_1 = 0

    for letra in mac_add:
        if letra != ":":
            if letra not in validar:
                cont+=1
                return False
        else:
            cont_1+=1
            if cont ==2:
                cont=0
            else:
                return False
                
    if cont_1 !=5:
        return False
    return True



