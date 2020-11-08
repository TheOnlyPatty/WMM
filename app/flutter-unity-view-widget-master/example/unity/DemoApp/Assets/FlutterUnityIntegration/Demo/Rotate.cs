using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;
using System.Security.Cryptography;
using System;
using UnityEngine.EventSystems;

public class Rotate : MonoBehaviour, IEventSystemHandler
{
    public float w1 = 1;
    public float x1 = 0;
    public float y1 = 0;
    public float z1 = 0;
    public float w2 = 1;
    public float x2 = 0;
    public float y2 = 0;
    public float z2 = 0;
    // Start is called before the first frame update

    public h other;

    [SerializeField]
    Quaternion quat;

    void Start()
    {
        transform.rotation = Quaternion.FromToRotation(Vector3.up, transform.forward);
    }

    // Update is called once per frame
    void Update()
    {
        //transform.rotation = quat;
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
        w1 = float.Parse(a[0]);
        x1 = float.Parse(a[1]);
        y1 = float.Parse(a[2]);
        z1 = float.Parse(a[3]);
        w2 = float.Parse(a[4]);
        x2 = float.Parse(a[5]);
        y2 = float.Parse(a[6]);
        z2 = float.Parse(a[7]);

        transform.rotation = Change(w1, x1, y1, z1);
        other.DoSomething(w2, x2, y2, z2);
    }
}
