# ! /bin/sh

for i in ./bin/* do

    echo test de "$(basename $i)"
    
    for j in ./ressources/dicos/* do 
        for k in ./ressources/textes/* do
        echo avec "$(basename $j)" et "$(basename $k)"
        output_path="./resultats/$(basename $i)AVEC$(basename $j)ET$(basename $k)"
        "$i" "$j" "$k" > "$output_path"
        done
    done
done
