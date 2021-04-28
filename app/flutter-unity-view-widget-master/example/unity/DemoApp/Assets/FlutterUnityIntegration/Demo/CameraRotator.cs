using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraRotator : MonoBehaviour
{
    public Transform target;

    // Update is called once per frame

    public void rotateCamera()
    {
        transform.rotation = target.rotation;
        transform.Rotate(180, 0, 0);
    }
}
