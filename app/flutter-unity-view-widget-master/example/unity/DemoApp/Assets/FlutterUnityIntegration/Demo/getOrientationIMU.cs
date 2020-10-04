using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;
using System.Security.Cryptography;
using System;
using UnityEngine.EventSystems;

public class getOrientationIMU : MonoBehaviour, IEventSystemHandler
{
    public float w;
    public float x;
    public float y;
    public float z;
    // Start is called before the first frame update

    [SerializeField]
    Quaternion quat;

    void Start()
    {
        transform.rotation = Quaternion.FromToRotation(Vector3.up, transform.forward);
    }

    // Update is called once per frame
    void Update()
    {
        transform.rotation = quat;
    }

    private static Quaternion Change(float w, float x, float y, float z)
    {
        Quaternion newQuat = new Quaternion();
        newQuat.Set(x, y, z, w);
        return newQuat;

    }

    public void SetOrientation(string message)
    {

        message = message.Trim(';');
        string[] a = message.Split(',');
        w = float.Parse(a[0]);
        x = float.Parse(a[1]);
        y = float.Parse(a[2]);
        z = float.Parse(a[3]);
        quat = Change(w, x, y, z);
    }
}
