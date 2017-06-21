﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public static class SoundManager
{
    public enum FadeType { FADEOUT = -1, NONE, FADEIN, FADEINOUT }

    static GameObject Controller;
    public static AudioSource BGM;
    static AudioSource Crossfade;
    static List<AudioSource> SFX;
    static FadeType Fade = FadeType.NONE;
    static LoopData LoopStats;
    static string CurrentTitle;
    static SavedBGM PausedAudio;
    // Fade over about 3 seconds.
    const float FADESPEED = 0.00555555555555555555555555555556f;

    static bool isPaused = false;
    public static float MaxBGMVolume = 0.5f;
    public static float MaxSFXVolume = 1;
    public static bool Initialized = false;
    public struct LoopData
    {
        public int start;
        public int end;

        public LoopData(int _start, int _end)
        {
            start = _start;
            end = _end;
        }

        public static bool operator ==(LoopData l1, LoopData l2)
        {
            return l1.start == l2.start && l1.end == l2.end;
        }

        public static bool operator !=(LoopData l1, LoopData l2)
        {
            return l1.start != l2.start || l1.end != l2.end;
        }

        public override bool Equals(object o)
        {
            return o == (object)this;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
    }
    public struct SavedBGM
    {
        public string title;
        public AudioClip clip;
        public LoopData loop;
        public float volume;
        public int sample;
    }


    static Dictionary<string, LoopData> Loops = new Dictionary<string, LoopData>();

    public static void Init(GameObject _controller)
    {
        // Set the controller object that will house the actual audio sources, and add them sources.
        if (Initialized)
        {
            return;
        }
        Initialized = true;
        Controller = _controller;
        BGM = Controller.AddComponent<AudioSource>();
        Crossfade = Controller.AddComponent<AudioSource>();
        SFX = new List<AudioSource>();
        BGM.playOnAwake = false;
        BGM.loop = true;
        Crossfade.playOnAwake = false;
        Crossfade.loop = true;

        // Manually add the loop information beause configuration files are hard.
        if (!Loops.ContainsKey("AMBIANCE"))
        {
            Loops.Add("AMBIANCE", new LoopData(0, 1940833));
            Loops.Add("DRUMS", new LoopData(0, 2570388));
            Loops.Add("SYNTHS", new LoopData(0, 2515920));
            Loops.Add("WEIRD N BASS", new LoopData(0, 2094594));
            Loops.Add("J_Game_Amb_Bright_loop_01", new LoopData(0, 2404985));
            Loops.Add("J_Game_Amb_Dark_loop_02", new LoopData(0, 1056500));
            Loops.Add("J_Game_Amb_Energy_1", new LoopData(0, 2227651));
            Loops.Add("nullsound", new LoopData(0, 1834968));
            Loops.Add("J_Game_Amb_Smooth", new LoopData(0, 1321853));
            Loops.Add("J_Game_Energyzz_4", new LoopData(0, 1940833));
            Loops.Add("J_Game_Oouuhh", new LoopData(0, 2570388));
            Loops.Add("J_Game_Starrs", new LoopData(0, 2515920));
            Loops.Add("J_Game_Strohvi", new LoopData(0, 2094594));
            Loops.Add("J_Game_Utterance", new LoopData(0, 2404985));
        }
    }

    public static void Update()
    {
        if (!Controller)
        {
            return;
        }

        if (Fade != FadeType.NONE)
        {
            // Fade In or Out, change the volume level.
            if (Fade == FadeType.FADEIN || Fade == FadeType.FADEOUT)
            {
                BGM.volume = Mathf.Clamp(BGM.volume + ((int)Fade * FADESPEED), 0, MaxBGMVolume);
                if (BGM.volume == 0 || BGM.volume == MaxBGMVolume)
                {
                    Fade = FadeType.NONE;
                    if (BGM.volume == 0)
                    {
                        BGM.Stop();
                        BGM.timeSamples = 0;
                    }
                }
                // Crossfade - Fade in AND out.
            }
            else if (Fade == FadeType.FADEINOUT)
            {
                BGM.volume -= FADESPEED;
                Crossfade.volume = Mathf.Clamp(Crossfade.volume + FADESPEED, 0, MaxBGMVolume);
                if (Crossfade.volume == MaxBGMVolume)
                {
                    // Swap the BGM and Crossfade AudioSources so that there is no audio interruption
                    // switching the track to the other source.
                    AudioSource tmp = BGM;
                    BGM = Crossfade;
                    Crossfade = tmp;
                    Crossfade.Stop();
                    Crossfade.clip = null;
                    Fade = FadeType.NONE;
                }
            }
        }
        else
        {
            // Fade type is NONE
            if (BGM.volume != MaxBGMVolume)
            {
                BGM.volume = MaxBGMVolume;
            }
        }

        // Handle BGM looping
        if (BGM.timeSamples > LoopStats.end && BGM.pitch >= 0)
        {
            BGM.timeSamples = LoopStats.start;
        }
        else if ((BGM.timeSamples > LoopStats.end || BGM.timeSamples < LoopStats.start) && BGM.pitch < 0)
        {
            BGM.timeSamples = LoopStats.end;
        }

        // Find and kill dead SFX audio components.
        for (int i = SFX.Count - 1; i >= 0; i--)
        {
            if (!(SFX[i].isPlaying) && isPaused == false)
            {
                Object.Destroy(SFX[i]);
                SFX.RemoveAt(i);
            }
        }
    }

    public static void DestroySFX()
    {
        if (SFX != null)
        {
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                // if (!(SFX[i].isPlaying) && isPaused == false)
                {
                    if (SFX[i] != null)
                        Object.Destroy(SFX[i]);
                    SFX.RemoveAt(i);
                }
            }
        }

    }
    public static void Play(string _title)
    {
        if (!Controller)
        {
            return;
        }
        if (CurrentTitle == _title)
        {
            return;
        }
        BGM.Stop();
        Fade = FadeType.NONE;
        if (Loops.ContainsKey(_title))
            LoopStats = Loops[_title];
        BGM.volume = MaxBGMVolume;
        BGM.clip = (AudioClip)Resources.Load("BGM/" + _title);
        BGM.timeSamples = 0;
        BGM.Play();
        CurrentTitle = _title;
    }

    public static void Pause()
    {
        if (!Controller)
        {
            return;
        }
        PausedAudio = SaveBGM();
        //CrossFadeBGM("Pause");
    }

    public static void Resume()
    {
        if (!Controller)
        {
            return;
        }
        if (PausedAudio.clip != null)
        {
            ReplayBGM(PausedAudio);
        }
    }

    public static void FadeInBGM(string _title, bool _cross = true)
    {
        if (!Controller)
        {
            return;
        }

        if (CurrentTitle == _title)
        {
            return;
        }
        if (BGM.isPlaying && _cross)
        {
            CrossFadeBGM(_title);
            return;
        }
        CurrentTitle = _title;
        Fade = FadeType.FADEIN;
        AudioClip c = (AudioClip)Resources.Load("BGM/" + _title);
        if (Loops.ContainsKey(_title))
            LoopStats = Loops[_title];
        BGM.volume = 0;
        BGM.clip = c;
        BGM.timeSamples = 0;
        BGM.Play();
    }

    public static void FadeOutBGM()
    {
        if (!Controller)
        {
            return;
        }
        if (isPlaying())
        {
            Fade = FadeType.FADEOUT;
        }
    }
    public static AudioSource GetSFXSource(string name)
    {
        for(int i = 0; i < SFX.Count; i++)
        {
            if(SFX[i].clip.name == name)
            {
                return SFX[i];
            }

        }
        return null;
    }
    public static void PlaySFX(string _title)
    {
        if (Controller)
        {
            AudioSource source = Controller.AddComponent<AudioSource>();
            source.clip = (AudioClip)Resources.Load("SFX/" + _title);
            if (source.clip == null)
            {
                Debug.Log("Sound '" + _title + "' was not found in the SFX folder!");
            }
            source.volume = MaxSFXVolume;
            source.Play();
            SFX.Add(source);
        }
    }
    public static void PauseSFX()
    {
        if (SFX != null)
        {
            for (int i = 0; i < SFX.Count; i++)
            {
                if (SFX[i] != null)
                    SFX[i].Pause();
            }
        }

        isPaused = true;
    }
    public static void UnPauseSFX()
    {
        if (SFX != null)
        {
            for (int i = 0; i < SFX.Count; i++)
            {
                if (SFX[i] != null)
                    SFX[i].UnPause();
            }
        }
        isPaused = false;
    }
    public static void PlayVirusVoice(string _title)
    {
        if (Controller)
        {
            AudioSource source = Controller.AddComponent<AudioSource>();
            source.clip = (AudioClip)Resources.Load("SFX/Virus Gameplay Lines/" + _title);
            if (source.clip == null)
            {
                Debug.Log("Sound '" + _title + "' was not found in the SFX folder!");
            }
            source.volume = MaxSFXVolume;
            source.Play();
            SubstitlesScript.voice = source;
            SFX.Add(source);
        }
    }
    public static bool IsVirusVoicePlaying(string _title)
    {
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/Virus Gameplay Lines/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    return true;
                }
            }
        }
        return false;
    }
    public static bool StopVirusVoice(string _title, bool _all = false)
    {
        bool stopped = false;
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/Virus Gameplay Lines/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    SFX[i].Stop();
                    if (!_all)
                    {
                        return true;
                    }
                    else
                    {
                        stopped = true;
                    }
                }
            }
        }
        return stopped;
    }
    public static void PlayCellVoice(string _title)
    {
        if (Controller)
        {
            AudioSource source = Controller.AddComponent<AudioSource>();
            source.clip = (AudioClip)Resources.Load("SFX/Medical VR Cell Lines/" + _title);
            if (source.clip == null)
            {
                Debug.Log("Sound '" + _title + "' was not found in the SFX folder!");
            }
            source.volume = MaxSFXVolume;
            source.Play();
            SubstitlesScript.voice = source;
            SFX.Add(source);
        }
    }
    public static bool IsCellVoicePlaying(string _title)
    {
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/Medical VR Cell Lines/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    return true;
                }
            }
        }
        return false;
    }
    public static bool StopCellVoice(string _title, bool _all = false)
    {
        bool stopped = false;
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/Medical VR Cell Lines/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    SFX[i].Stop();
                    if (!_all)
                    {
                        return true;
                    }
                    else
                    {
                        stopped = true;
                    }
                }
            }
        }
        return stopped;
    }

    public static void PlayJordanVoice(string _title)
    {
        if (Controller)
        {
            AudioSource source = Controller.AddComponent<AudioSource>();
            source.clip = (AudioClip)Resources.Load("SFX/StrategyGame/" + _title);
            if (source.clip == null)
            {
                Debug.Log("Sound '" + _title + "' was not found in the SFX folder!");
            }
            source.volume = MaxSFXVolume;
            source.Play();
            SubstitlesScript.voice = source;
            SFX.Add(source);
        }
    }
    public static bool IsJordanPlaying(string _title)
    {
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/StrategyGame/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    return true;
                }
            }
        }
        return false;
    }
    public static bool StopJordanVoice(string _title, bool _all = false)
    {
        bool stopped = false;
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/StrategyGame/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    SFX[i].Stop();
                    if (!_all)
                    {
                        return true;
                    }
                    else
                    {
                        stopped = true;
                    }
                }
            }
        }
        return stopped;
    }




    public static bool IsSFXPlaying(string _title)
    {
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    return true;
                }
            }
        }
        return false;
    }

    public static bool stopSFX(string _title, bool _all = false)
    {
        bool stopped = false;
        if (Controller)
        {
            AudioClip c = (AudioClip)Resources.Load("SFX/" + _title);
            for (int i = SFX.Count - 1; i >= 0; i--)
            {
                if (SFX[i].clip == c)
                {
                    SFX[i].Stop();
                    if (!_all)
                    {
                        return true;
                    }
                    else
                    {
                        stopped = true;
                    }
                }
            }
        }
        return stopped;
    }

    public static void CrossFadeBGM(string _title)
    {
        if (!Controller)
        {
            return;
        }
        if (CurrentTitle == _title)
        {
            return;
        }
        AudioClip c = (AudioClip)Resources.Load("BGM/" + _title);
        if (c != null)
        {
            CurrentTitle = _title;
            if (Loops.ContainsKey(_title))
                LoopStats = Loops[_title];
            Fade = FadeType.FADEINOUT;
            Crossfade.Stop();
            Crossfade.clip = c;
            Crossfade.volume = 0;
            Crossfade.Play();
        }
    }

    public static SavedBGM SaveBGM()
    {
        SavedBGM s = new SavedBGM();
        if (Controller)
        {
            s.title = CurrentTitle;
            s.clip = BGM.clip;
            s.loop = LoopStats;
            s.volume = BGM.volume;
            s.sample = BGM.timeSamples;
        }
        return s;
    }

    public static void ReplayBGM(SavedBGM s)
    {
        if (!Controller)
        {
            return;
        }
        BGM.Stop();
        BGM.clip = s.clip;
        BGM.timeSamples = s.sample;
        BGM.volume = s.volume;
        BGM.Play();
        CurrentTitle = s.title;
        LoopStats = s.loop;
    }

    public static bool isPlaying()
    {
        if (!Controller)
        {
            return false;
        }
        return BGM.isPlaying;
    }

    public static float GetBGMPitch()
    {
        return BGM.pitch;
    }

    public static void SetBGMPitch(float p)
    {
        BGM.pitch = p;
    }
}
