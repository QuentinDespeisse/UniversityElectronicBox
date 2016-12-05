using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.Threading;
using System.Windows.Threading;
using System.Windows.Controls.Primitives;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;

namespace UEB
{
    
     
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Dictionary<int, string> questions = new Dictionary<int,string>();
        private int numQuestion = 1;
        static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        string[] reponses = new string[3];
        private int delaiReponse = 10;
        private List<Rectangle> statsBarres;
        private List<Label> statsLabels;
        
        // Déclaration d'un objet UdpClient qui servira a initier la connexion à l'Arduino
        UdpClient udpClient;

        public MainWindow()
        {
            InitializeComponent();
            
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) //S'exécute au lancement de l'application
        {
            // Déclaration d'un objet IPAddress afin de stocker l'adresse IP
            IPAddress AdresseIP = IPAddress.Parse("192.168.1.15");
            // Déclaration d'un objet IPEndPoint (Socket : Adresse IP + Numéro du port saisie dans
            // la TextBox)
            IPEndPoint AdresseIP_Port = new IPEndPoint(IPAddress.Broadcast, Convert.ToInt32("2390"));
            // Connexion UDP via le socket
            udpClient = new UdpClient();
            udpClient.Connect(AdresseIP_Port);

            //On range les rectangles et les label dans des List pour y accéder plus efficacement
            statsBarres = new List<Rectangle> { rect1, rect2, rect3, rect4, rect5 };
            statsLabels = new List<Label> { lab1, lab2, lab3, lab4, lab5 };

            //On rend invisible les éléments d'affichage des réponses
            foreach (Rectangle rect in statsBarres)
            {
                rect.Visibility = Visibility.Hidden;
            }

            foreach (Label lab in statsLabels)
            {
                lab.Visibility = Visibility.Hidden;
            }
            
        } 
        

        private void WindowClosing(object sender, System.ComponentModel.CancelEventArgs e) //A la fermeture de la fenetre
        {
            /*
            Byte[] envoiOctets = Encoding.ASCII.GetBytes("7c0");
            udpClient.Send(envoiOctets, envoiOctets.Length);
            */
        }

        private void AddQuestion(object sender, RoutedEventArgs e)
        {
            // On vérifie quelles sont les réponses justes pour cette question
            string rep = "";
            if ((bool) repA.IsChecked) {
                rep += 'A';
            }
            if ((bool) repB.IsChecked)
            {
                rep += 'B';
            }
            if ((bool) repC.IsChecked)
            {
                rep += 'C';
            }
            if ((bool) repD.IsChecked)
            {
                rep += 'D';
            }

            // On enregistre les bonnes réponses dans notre Dictionnary
            questions[numQuestion] = rep;

            // On ajoute la possibilité de pouvoir poser cette question dans la comboBox
            if (nomRep.Text.Length > 0)
                listeQuestions.Items.Add(numQuestion + " : " + nomRep.Text);
            else
                listeQuestions.Items.Add("Question n°" + numQuestion);

            // On passe à la questn suivante
            numQuest.Content = "Question n°" + ++numQuestion;

            // On réinitialise l'état des checkBox
            repA.IsChecked = false;
            repB.IsChecked = false;
            repC.IsChecked = false;
            repD.IsChecked = false;

        }

        private void PoserQuestion(object sender, RoutedEventArgs e)
        {
            foreach (Label lab in statsLabels)
            {
                lab.Visibility = Visibility.Hidden;
            }
            foreach (Rectangle rect in statsBarres)
            {
                rect.Visibility = Visibility.Hidden;
            }

            //On récupère la question sélecionnée dans la ComboBox
            string quest = (string) listeQuestions.SelectedValue;
            //On extrait le numéro de la string afin de rendre la donnée compatible avec notre Dictionnary
            int numero = Int32.Parse(quest.Remove(0, 11));

            //On envoie le signal aux télécommandes
            Byte[] envoiOctets = Encoding.ASCII.GetBytes("newQuest");
            udpClient.Send(envoiOctets, envoiOctets.Length);

            //Paramètrage pour recevoir des données sur le port 11000 depuis n'importe quelle IP, on lui impose un timeout de 1s
            UdpClient receivingUdpClient = new UdpClient(11000);
            IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
            receivingUdpClient.Client.ReceiveTimeout=delaiReponse*1000;

            //Initialisation du nombre de personnes ayant répondu à 0
            int nbRep = 0;

            //Création du dictionnary qui contiendra les réponses pour chaque combinaison possible
            Dictionary<String, int> currentAnswers = new Dictionary<string, int>();

            // On démarre le chronomètre
            Stopwatch s = new Stopwatch();
            s.Start();

            //Tant que le delai selectionné par l'utilisateur n'est pas dépassé
            while (s.Elapsed < TimeSpan.FromSeconds(delaiReponse))
            {
                try
                {
                    //On récupère les données d'une télécommande
                    Byte[] receptionOctets = receivingUdpClient.Receive(ref RemoteIpEndPoint);
                    string rep = Encoding.UTF8.GetString(receptionOctets);
                    //On incrémente le nombre de réponse
                    ++nbRep;

                    //Si la réponse a déjà été donnée on l'incrémente, sinon on la crée et on l'initialise à 1
                    if (currentAnswers.ContainsKey(rep))
                        ++currentAnswers[rep];
                    else
                        currentAnswers.Add(rep, 1);
                }
                catch (SocketException)
                {
                }


            }
            s.Stop(); //On stop le chrono

            //On récupère les 5 réponses ayant obtenu le plus de réponses
            var top5 = currentAnswers.OrderByDescending(pair => pair.Value).Take(5).ToDictionary(pair => pair.Key, pair => pair.Value);



            int Cpt = 0;
            //On affiche les barres de stat pour chacunes de ces réponses
            foreach (KeyValuePair<String, int> val in top5)
            {
                double taille = ((double) val.Value / (double) nbRep) * (double) 300;
                statsBarres[Cpt].Width = taille;
                if (val.Key == questions[numero])
                    statsBarres[Cpt].Fill = new SolidColorBrush(Color.FromRgb(0, 255, 0));
                else
                    statsBarres[Cpt].Fill = new SolidColorBrush(Color.FromRgb(255, 0, 0));
                statsBarres[Cpt].Visibility = Visibility.Visible;
                statsLabels[Cpt].Content = "Réponse " + val.Key + " (" + Math.Round(((double)val.Value / (double)nbRep) * (double)100, MidpointRounding.AwayFromZero) + "%)";
                statsLabels[Cpt].Visibility = Visibility.Visible;
                ++Cpt;
            }
            

        }



        private void listeQuestions_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //Si on sélectionne une question dans la liste alors on peut activer le bouton
            poserQuestion.IsEnabled = true;
        }

        private void delai_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //Lorsque l'utilisateur change la valeur de delai, on applique le changement sur la variabe et sur le label
            int val = Convert.ToInt32(delai.Value);
            delaiLabel.Content = val + " s";
            delaiReponse =val;
        }


        }


        

    }

