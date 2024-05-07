"""
TIE-02101 Johdatus ohjelmointiin kesä 2019 / Introduction to programming Summer 2019
Suvi Suominen, studentID: 284432, suvi.suominen@tuni.fi

Ohjelma on yksinkertainen esitys hirsipuu-pelistä, jossa käyttäjä joko syöttää
tekstikenttään arvattavan sanan, tai painaa random-nappia, jolloin ohjelma arpoo
arvattavan sanan valmiista listasta.
Sanan kirjaimia arvataan painamalla kirjainta vastaavaa nappia, jolloin ohjelma ilmoittaa,
onko arvaus oikein (näytetään kirjain) vai väärin (piirretään hirsipuuta).
Vääriä arvauksia voi olla maksimissaan 13, joiden jälkeen peli päättyy.
Käyttäjä voi myös luovuttaa pelin, jolloin arvattava sana paljastetaan koko-
naisuudessaan.

Ohjelmassa on tähdätty skaalautuvaan käyttöliittymään.
"""

from tkinter import *
import random

# hirsipuun piirtämiseen tarvittavat kuvatiedostot
KUVATIEDOSTOT = ["gallows_1.gif","gallows_2.gif","gallows_3.gif","gallows_4.gif",
                 "gallows_5.gif","gallows_6.gif","gallows_7.gif","gallows_8.gif",
                 "gallows_9.gif","gallows_10.gif","gallows_11.gif","gallows_12.gif",
                 "gallows_13.gif"]

# aakkoset, joita tarvitaan kirjainten arvailuun ja nappeihin
AAKKOSET = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n",
            "o","p","q","r","s","t","u","v","w","x","y","z","ä","ö","å","-"]

# valmiita sanoja, jos käyttäjä haluaa pelata yksin, ilman että ohjelmaan
# syötetään arvattava sana
SANOJA = ["tammenterho","teekkarilakki","älypuhelin","kiikkutuoli","python-ohjelmointi",
          "åbo","herwood","skellari","lakitus","mikontalo","gambinakokous",
          "matkakaiutin","nistipata","alko","työttömyyskassa","tampereentero",
          "jodelkarma","rauduskoivu","rytmimusiikki","pakkoruotsi","wäinölät",
          "tupsufuksi","sååsbar"]

class Hirsipuu:
    """ Luokka edustaa yksinkertaista sananarvailupeliä, jossa käyttäjä
    arvaa piilotetun sanan kirjaimia kirjainnappeja painamalla
    """

    def __init__(self):


        self.__window = Tk()
        self.__window.geometry("950x600+300+0")
        # siirretään ikkunaa oikealle, jolloin pythonin suoritusikkuna ja mah-
        # dollinen virheilmoitus näkyvät

        self.__top_frame = Frame(self.__window)
        self.__center_frame = Frame(self.__window)
        self.__bottom_frame = Frame(self.__window)
        self.__top_frame.pack(side="top")
        self.__center_frame.pack()
        self.__bottom_frame.pack()
        # jaetaan pääikkuna osiin, jotta käyttöliittymän asettelu helpottuu

        self.__window.title("Hangman")
        self.__window.option_add("*Font", "Verdana 12")

        # pyydetään käyttäjältä arvattava sana tai ohjeistetaan
        # klikkaamaan nappia, jolloin käyttäjälle annetaan jokin sana arvattavaksi
        # valmiista sanalistasta
        self.__ohjelabel_1 = Label(self.__top_frame,
                        text="Click the Random-button for a single player game,")
        self.__ohjelabel_2 = Label(self.__top_frame,
                                   text="or enter the word to be guessed: ")
        self.__arvattava_sana = Entry(self.__top_frame)


        self.__ohjelabel_1.pack(side="top")
        self.__ohjelabel_2.pack(side="left")
        self.__arvattava_sana.pack(side="left")

        # luodaan kaikki tarvittavat napit
        self.__start_button = Button(self.__center_frame,text="Start",
                                     command=self.start,background="light green")
        self.__start_button.pack(side="top")

        self.__random_button = Button(self.__top_frame, text="Random",
                                      command=self.random,background="light yellow")
        self.__random_button.pack(side="right")

        self.__new_game_button = Button(self.__top_frame, text="New game",
                                        command=self.new_game,background="light blue")
        self.__new_game_button.pack(side="left")

        self.__stop_button = Button(self.__top_frame,text="I give up",
                                    command=self.give_up,background="pink")
        self.__stop_button.pack(side="right")


        self.__pelitilanne = Label(self.__center_frame)
        self.__pelitilanne.pack(side="top")
        # labeli, jolla käyttäjälle voidaan antaa virheilmoituksia
        # ja ilmoittaa, kun peli päättyy


        # muodostetaan .gif kuvista PhotoImage olioita,
        # jotta niitä voidaan hyödyntää myöhemmin ohjelmassa
        self.__hirsipuukuvat = []
        for kuvatiedosto in KUVATIEDOSTOT:
            kuva = PhotoImage(file=kuvatiedosto)
            self.__hirsipuukuvat.append(kuva)

        self.__tyhjäkuva = PhotoImage(width=250,height=350)
        # tyhjä kuva, joka edustaa piirtämätöntä hirsipuuta
        # tämän avulla voidaan kuvalabel asetella ohjelmaan sopivasti
        self.__kuvalabel = Label(self.__center_frame)
        self.__kuvalabel.pack(side="right")


        self.kirjaimille_napit()
        # kutsutaan funktiota, joka rakentaa kirjainnapit

        self.__monesko_väärä = -1
        self.__arvatut_kirjaimet = 0
        # pidetään kirjaa siitä, kuinka monta kertaa sanan kirjaimia on arvattu väärin
        # (tai oikein) jotta hirsipuu saadaan piirettyä myöhemmin ohjelmassa
        # (tai voidaan ilmoittaa voitto)

    def start(self):
        """ erotellaan annetusta sanasta kirjaimet, ja luodaan niille uudet
        labelit (kutsumalla funktiota), joiden paikalle arvattuja kirjaimia
        myöhemmin paljastetaan
        tehdään myös syötetarkastelu
        """

        try:
            if len(self.__arvattava_sana.get()) != 0:
                sana = self.__arvattava_sana.get()

            else:
                # jos sana on arvottu, eli Entry()-palauttaa tyhjää
                sana = self.__random_sana

            self.__lista_kirjaimista = []
            # erotellaan kirjaimet sanasta listan alkioiksi
            self.__kirjainlabelit = []
            # lista labeleille, joiden paikalle arvatut kirjaimet tulevat

            for kirjain in sana:
                if kirjain in AAKKOSET:
                    self.__lista_kirjaimista.append(kirjain)

            if len(sana) == 0:
                self.__pelitilanne.configure(text="The word cannot be blank")

            elif len(sana) > 23:
                self.__pelitilanne.configure(text="The word cannot be "
                                                      "over 23 characters")
                self.__lista_kirjaimista = []

            elif len(self.__lista_kirjaimista) != len(sana):
                # jos sanassa esiintyy tuntemattomia kirjaimia
                self.__pelitilanne.configure(text="The word contains "
                                                      "unknown characters")
                self.__lista_kirjaimista = []

            else:
                self.__pelitilanne.configure(text="")
                self.__start_button.configure(state=DISABLED,
                                                          background="light grey")
                self.__random_button.configure(state=DISABLED,
                                                           background="light grey")
                # deaktivoidaan napit, joilla käyttäjä valitsee uuden sanan
                # sillä niille ei ole käyttöä kesken pelin
                self.arvattavalle_sanalle_labelit()

            try:
                # tyhjennetään tekstikenttä, jos siinä on ollut tekstiä
                # eli hypätään yli, jos sana on arvottu käyttäjälle
                self.__arvattava_sana.delete(0, END)
            except AttributeError:
                pass

        except AttributeError:
            # jos start-nappia yritetään painaa, ennen kuin ohjelmalle on annettu
            # arvattava sana
            self.__pelitilanne.configure(text=
                                         "Please enter a word or press Random")


    def random(self):
        """ arvotaan listasta valmis sana, kun random-nappia on painettu """

        self.__random_sana = random.choice(SANOJA)
        self.start()


    def arvattavalle_sanalle_labelit(self):
        """ muodostetaan ensin tyhjää näyttävät labelit jokaista sanan
        kirjainta vastaan, joiden paikalle sanan oikein arvatut kirjaimet
        tulevat näkyviin """

        for i in range(len(self.__lista_kirjaimista)):
            self.__kirjainlabel = Label(self.__center_frame,text=" _ ",
                                        background="light blue")
            self.__kirjainlabelit.append(self.__kirjainlabel)
            self.__kirjainlabel.pack(side="left")


    def kirjaimille_napit(self):
        """ luodaan kirjainnapit, joita painamalla käyttäjä arvaa
        sanan kirjaimia """

        self.__kirjainnapit = {}
        for kirjain in AAKKOSET:
            self.__kirjainnappi = Button(self.__bottom_frame,text=kirjain,
                    command = lambda kirjain=kirjain: self.onko_oikein(kirjain),
                    background="light blue",height=2,width=2)

            self.__kirjainnappi.pack(side="left")
            self.__kirjainnapit[kirjain] = self.__kirjainnappi


    def onko_oikein(self,kirjain):
        """ tarkistetaan, onko arvattu kirjain oikein, eli esiintyykö se
        sanassa ainakin yhden kerran
        :param: kirjain, jota vastaavaa nappia on painettu
        """

        try:
            if self.__lista_kirjaimista == []:
                # jos kirjainnappeja painellaan, ilman että arvattavaa sanaa on
                # olemassa ohjelmassa sillä hetkellä
                pass

            else:
                if kirjain not in self.__lista_kirjaimista:
                    # jos arvattu kirjain ei esiinny kertaakaan sanassa, ilmoitetaan tästä
                    # käyttäjälle, ja piirretään hirsipuusta seuraava kuva
                    # ja laitetaan nappi pois päältä

                    self.__kirjainnapit[kirjain].configure(state=DISABLED,
                                                           background="pink")
                    self.__monesko_väärä += 1

                    if self.__monesko_väärä < 13:
                        # ohjelmassa sallitaan max 13 väärää arvausta
                        # joiden jälkeen peli päättyy
                        self.__pelitilanne.configure(text="Wrong guess, you have "
                                    +str(13-self.__monesko_väärä)+" guess(es) left.")

                        self.__kuvalabel.configure(image=
                                    self.__hirsipuukuvat[self.__monesko_väärä])

                    else:
                        self.__pelitilanne.configure(text="Game Over")
                        self.give_up()

                else:
                    # käydään sanan kirjaimia läpi, ja katsotaan vastaako
                    # jokin niistä arvattua kirjainta, ja jos näin on,
                    # muutetaan se käyttäjälle näkyväksi ja muutetaan nappi vihreäksi

                    self.__pelitilanne.configure(text="")
                    # poistetaan ilmoitus väärästä arvauksesta

                    self.__kirjainnapit[kirjain].configure(state=DISABLED,
                                    background="light green")
                    for i in range(len(self.__lista_kirjaimista)):
                        if kirjain == self.__lista_kirjaimista[i]:
                            self.__kirjainlabelit[i].configure(text="{:3s}".
                                                        format(kirjain))
                            self.__arvatut_kirjaimet += 1

                    if self.__arvatut_kirjaimet == len(self.__lista_kirjaimista):
                        self.__pelitilanne.configure(text="You won!")
                        self.give_up()

        except IndexError:
            pass
        except AttributeError:
            # jos kirjainnappeja painetaan, ennen kuin peli on aloitettu
            pass
        except TclError:
            pass

    def give_up(self):
        """ Jos käyttäjä ei arvaa sanaa, vaan luovuttaa pelin,
        paljastetaan sanan kirjaimet. Tätä funktiota kutsutaan myös,
        kun pelaaja on joko voittanut tai hävinnyt pelin """

        if len(self.__lista_kirjaimista) == 0:
            pass

        else:
            try:
                for self.__kirjainlabel in self.__kirjainlabelit:
                    indeksi = self.__kirjainlabelit.index(self.__kirjainlabel)
                    self.__kirjainlabel.configure(text="{:3s}"
                                    .format(self.__lista_kirjaimista[indeksi]))

                self.__lista_kirjaimista = []
                # tyhjennetään sana muistista

            # hypätään funktion yli, jos käyttäjä yrittää luovuttaa ennen kuin
            # peli on aloitettu
            except TclError:
                pass
            except AttributeError:
                pass


    def new_game(self):
        """ jos käyttäjä haluaa aloittaa uuden pelin
        poistetaan nykyinen sana näytöltä, ja aktivoidaan
        tarvittavat napit uudelleen, sekä tyhjennetään pelitilanne-teksti
        ja hirsipuukuva """

        self.__monesko_väärä = -1 # asetetaan takaisin alkuarvoon
        self.__arvatut_kirjaimet = 0

        try:
            self.__lista_kirjaimista = [] # tyhjennetään sana muistista
            for label in self.__kirjainlabelit:
                label.destroy()

            self.__start_button.configure(state=NORMAL,background="light green")
            self.__random_button.configure(state=NORMAL,background="light yellow")

            self.__kuvalabel.configure(image=self.__tyhjäkuva)
            self.__pelitilanne.configure(text="")

        except AttributeError:
            # jos new game-nappia yritetään painaa, ennen kuin yhtäkään peliä
            # ollaan aloitettu
            pass

        for kirjain in self.__kirjainnapit:
            self.__kirjainnapit[kirjain].configure(state=NORMAL,
                                                   background="light blue")


    def establish_GUI(self):
        """ aloitetaan ohjelman suoritus"""
        self.__kuvalabel.configure(image=self.__tyhjäkuva)
        self.__window.mainloop()


def main():

    project = Hirsipuu()
    project.establish_GUI()

main()