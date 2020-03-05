using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.Data.Sqlite;
using Figgle;


namespace ExtractTool
{
    public class MainProgram
    {
        public static string g_sFilename = "";
        public static string g_sOutputFolder = "";
        public static void Main(string[] args)
        {
            Console.WriteLine("Starting..");
            try
            {

                Console.WriteLine(FiggleFonts.Standard.Render("ZNI Extract v1.2"));
            }catch(Exception e)
            {
                
            }

            Console.WriteLine();
            bool iHasSMS = false;
            SqliteConnection con = null;
            while (!iHasSMS)
            {

                Console.Write("What is the path to the 3d0d7e5fb2ce288813306e4d4636395e047a3d28 [help/%filename]: ");
                string filename = Console.ReadLine();

                if (filename == "help")
                {
                    Console.WriteLine("3d0d7e5fb2ce288813306e4d4636395e047a3d28 is typically located in the following locations:");
                    Console.WriteLine("Windows: \n* %APPDATA%/Apple/MobileSync/Backup/DEVICE_ID/3d\n* %USERPROFILE%/Apple/MobileSync/Backup/DEVICE_ID/3d");
                    Console.WriteLine("Mac: ~/Library/Application Support/MobileSync/Backup/DEVICE_ID/3d");
                    Console.WriteLine();
                }
                else
                {
                    if (File.Exists(filename))
                    {
                        con = new SqliteConnection($"Data Source='{filename}'");
                        con.Open();
                        iHasSMS = true;
                        g_sFilename = filename;
                    }
                    else
                    {
                        Console.WriteLine("Please enter a valid filename to proceed **");
                    }
                }
                
            }

            iHasSMS = false;
            while (!iHasSMS)
            {
                Console.Write("What folder should the text messages be output to? ");
                g_sOutputFolder = Console.ReadLine();
                if (Directory.Exists(g_sOutputFolder))
                {
                    Console.WriteLine("This folder must not exist!");
                    Console.WriteLine();
                }
                else
                {
                    Directory.CreateDirectory(g_sOutputFolder);
                    Directory.CreateDirectory(g_sOutputFolder + "/text");
                    Directory.CreateDirectory(g_sOutputFolder + "/html");

                    Console.WriteLine("Folder structure initialized");
                    iHasSMS = true;
                }
            }
            Console.WriteLine();

            string query = "select * from chat;";
            SqliteCommand cmd = new SqliteCommand(query, con);
            SqliteDataReader reader = cmd.ExecuteReader();
            while (reader.Read())
            {
                string handle="";
                string Service="";
                string ID="";
                for(int i = 0; i < reader.FieldCount; i++)
                {
                    string col = reader.GetName(i);
                    switch (col)
                    {
                        case "ROWID":
                            {
                                handle = reader.GetString(i);
                                break;
                            }
                        case "service_name":
                            {
                                Service = reader.GetString(i);
                                break;
                            }
                        case "chat_identifier":
                            {
                                ID = reader.GetString(i);
                                break;
                            }
                    }
                }

                Console.WriteLine("Building message history for " + ID);
                string get_msg = "select text, is_from_me, datetime(message.date / 1000000000 + 978307200,'unixepoch', 'localtime') as 'conv' from message where handle_id='" + handle + "';";

                string html_file = g_sOutputFolder + "/html/" + ID + ".html";
                string text_file = g_sOutputFolder + "/text/" + ID + ".txt";

                string template = File.ReadAllText("template.html");
                string orig = template;
                string raw_file = "";

                SqliteCommand msg = con.CreateCommand();
                msg.CommandText = get_msg;
                SqliteDataReader getter = msg.ExecuteReader();

                while (getter.Read())
                {
                    string text = "";
                    bool is_from_me = false;
                    string timestamp = "";
                    for(int ii = 0; ii < getter.FieldCount; ii++)
                    {
                        string _col = getter.GetName(ii);
                        switch (_col)
                        {
                            case "text":
                                {
                                    text = getter.GetString(ii);
                                    break;
                                }
                            case "is_from_me":
                                {
                                    int val = getter.GetInt32(ii);
                                    is_from_me = (val == 1) ? true : false;
                                    break;
                                }
                            case "conv":
                                {
                                    timestamp = getter.GetString(ii);
                                    break;
                                }
                        }

                        if (is_from_me)
                        {
                            raw_file += timestamp + " [ME]: " + text + "\n";
                            template += "addMsgFromMe('" + Base64Encode(text) + "', '" + timestamp + "');\n";
                        } else
                        {
                            raw_file += timestamp + " [" + ID + "]: " + text + "\n";
                            template += "addMsgFromThem('" + Base64Encode(text) + "', '" + timestamp + "');\n";
                        }
                    }
                    template += "</script></body></html>";
                    while (true)
                    {
                        try
                        {

                            File.WriteAllText(html_file, template);
                            File.WriteAllText(text_file, raw_file);
                            break;
                        }catch(Exception e)
                        {
                            Console.WriteLine("A file may be open in another program, press any key to try writing again!");
                            Console.ReadKey();
                        }
                    }

                    template = orig;
                }

                getter.Close();
            }

            reader.Read();
            con.Close();
            Console.WriteLine("* Database closed\n\nPress any key to exit.\n-> PROGRAM COMPLETE <-");
            Console.ReadKey();
        }


        public static string Base64Encode(string plainText)
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(plainText);
            return System.Convert.ToBase64String(plainTextBytes);
        }
        public static string Base64Decode(string base64EncodedData)
        {
            var base64EncodedBytes = System.Convert.FromBase64String(base64EncodedData);
            return System.Text.Encoding.UTF8.GetString(base64EncodedBytes);
        }
    }
}
