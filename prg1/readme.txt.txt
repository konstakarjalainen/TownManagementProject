Kaupungit oli helpoin totetuttaa structina, mikä mahdollistaa helpon yksilöllisten tietojen tallentamisen.

Kaikkien kaupunkien tietorakenteeksi valitsin unordered_map:in jonka avaimena on kaupungin id ja 
arvona osoite kaupungin structiin. Unordered_map oli tehokkain ratkaisu tietojen tallennukseen, koska id:t olivat
yksilöllisiä jolloin ne sopivat hyvin avaimiksi ja osoittimet on helppo tallentaa arvoiksi. Yksittäisiin kaupunkeihin
pääsee siis helposti käsiksi kun tiedetään halutun kaupungin id, jota käytettiin monessa funktiossa.

Järjestämiseen käytin heap sort:tia, mikä on asymptoottiselta tehokkuudeltaan kurssilla käydyistä algoritmeista 
paras merge sortin kanssa.

