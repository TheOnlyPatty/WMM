using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;
using UnityEngine;
using System.IO.Ports;
using System.Diagnostics;
using UnityEngine.UI;
using System.Security.Cryptography;

public class ReadIMU : MonoBehaviour
{

    // Start is called before the first frame update
    void Start()
    {

        sp.Open();
        sp.ReadTimeout = 1;
        transform.rotation = Quaternion.FromToRotation(Vector3.up, transform.forward);

    }


    SerialPort sp = new SerialPort("COM3", 9600);
    string buffer;
    float w = 1;
    float x = 0;
    float y = 0;
    float z = 0;
    int syscal = 0;
    int gyrocal = 0;
    int accelcal = 0;
    int magcal = 0;


    private static Quaternion Change(float w, float x, float y, float z)
    {
        Quaternion newQuat = new Quaternion();
        newQuat.Set(x, y, z, w);
        return newQuat;

    }
    // Update is called once per frame
    void Update()
    {
        
        if (sp.IsOpen)
        {

            try
            {
                while (true)
                {
                    buffer += System.Convert.ToChar(sp.ReadChar());

                    if (buffer.EndsWith(";") == true)
                    {
                        UnityEngine.Debug.Log(buffer);
                        string[] quatStringArray = buffer.Split(',');
                        w = float.Parse(quatStringArray[0]);
                        x = float.Parse(quatStringArray[1]);
                        y = float.Parse(quatStringArray[2]);
                        z = float.Parse(quatStringArray[3]);
                        syscal = int.Parse(quatStringArray[4]);
                        gyrocal = int.Parse(quatStringArray[5]);
                        accelcal = int.Parse(quatStringArray[6]);
                        magcal = int.Parse(quatStringArray[7].Trim(';'));

                        /*
                        UnityEngine.Debug.Log("w: " + w);
                        UnityEngine.Debug.Log("x: " + x);
                        UnityEngine.Debug.Log("y: " + y);
                        UnityEngine.Debug.Log("z: " + z);
                        UnityEngine.Debug.Log("sys: " + syscal);
                        UnityEngine.Debug.Log("gyro: " + gyrocal);
                        UnityEngine.Debug.Log("accel: " + accelcal);
                        UnityEngine.Debug.Log("mag: " + magcal);
                        */
                        
                        transform.rotation = Change(w, x, y, z);
                        

                        buffer = "";
                        break;
                    }
                }
                
                
                
                

            }

            catch (System.Exception)
            {
                throw;
            }

        }

    }


}