using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;
using UnityEngine;
using System.IO.Ports;
using System.Diagnostics;
using UnityEngine.UI;
using System.Security.Cryptography;

public class h : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        //transform.rotation = Quaternion.FromToRotation(Vector3.up, transform.forward);
    }

    private static Quaternion Change(float w, float x, float y, float z)
    {
        Quaternion newQuat = new Quaternion();
        newQuat.Set(x, y, z, w);
        return newQuat;

    }

    public void DoSomething(float w, float x, float y, float z)
    {
        try
        {
            transform.rotation = Change(w, x, y, z);
        }
        catch (System.Exception)
        {
            throw;
        }
    }
    // Update is called once per frame
    //void Update()
    //{
        
    //}
}
