﻿using UnityEngine;
using System.Collections;

public class CBullet : MonoBehaviour
{
    public float LifeSpan;
    //public float Speed;
    public GameObject Trial;

    void Start()
    {
        LifeSpan = 0.0f;
        //Speed = 6.0f;
        Instantiate(Trial, transform.position, transform.rotation, transform);
        Trial.GetComponent<ParticleSystem>().Play();
    }

    void Update()
    {
        LifeSpan += Time.deltaTime;
        if (LifeSpan >= 2.2f)
            Destroy(gameObject);
    }

    void FixedUpdate()
    {
        //GetComponent<Rigidbody>().velocity = transform.forward * Speed;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Target"))
        {
            GetComponent<MeshCollider>().enabled = false;
        }
    }
}
