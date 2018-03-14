using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PressurePlate : MonoBehaviour
{
    public bool shouldBePressed = false;
    public bool _opening = false;
    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
    }

    private void OnTriggerEnter(Collider collider)
    {
        if (collider.gameObject.tag == "Crate")
            _opening = true;
    }
    private void OnTriggerExit(Collider collider)
    {
        if (shouldBePressed)
        {
            if (collider.gameObject.tag == "Crate")
                _opening = false;
        }
    }
}
