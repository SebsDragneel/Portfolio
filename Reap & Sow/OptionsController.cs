using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class OptionsController : MonoBehaviour {

    // UI Components
    AudioSource sfx;
    //public UnityEngine.UI.Button FullscreenON, FullscreenOFF;
    //public UnityEngine.UI.Dropdown ResolutionDropdown;
    //public UnityEngine.UI.Text Index;
    public Slider BGMSlider, SFXSlider;
    [SerializeField]
    Selectable[] Elements;
    [SerializeField]
    int ElementIndex;
    //UnityEngine.UI.ColorBlock Solid, Transparent;
    
    float InitialSFXVolume, InitialBGMVolume;
    

	// Use this for initialization
	void Start () {
        // Get audio component for the SFX sound test.
        sfx = GetComponent<AudioSource>();
        ElementIndex = 0;

        // Get initial colors for the fullscreen buttons
        //   Fullscreen can be set on the initial start menu.
        //Solid = FullscreenON.colors;
        //Transparent = FullscreenON.colors;
        //Color hollow = Transparent.normalColor;
        //hollow.a = 0.5f;
        //Transparent.normalColor = hollow;
        //hollow.a = 0.7f;
        //Transparent.highlightedColor = hollow;

        // Set initial volumes
        InitialSFXVolume = PlayerPrefs.GetFloat("InitialSFXVolume", 1);
        InitialBGMVolume = PlayerPrefs.GetFloat("InitialBGMVolume", 1);

        BGMSlider.value = InitialBGMVolume;
        SFXSlider.value = InitialSFXVolume;
        SFXSlider.onValueChanged.AddListener(delegate { OnSFXChanged(SFXSlider.value); });

        // Populate the dropdown menu with the 
        // This shit hates me.
        //ResolutionDropdown.options.Clear();
        //for(int i = Screen.resolutions.Length - 1; i >= 0; i--) {
        //    ResolutionDropdown.options.Add(new UnityEngine.UI.Dropdown.OptionData(Screen.resolutions[i].ToString()));
        //    if(Screen.width == Screen.resolutions[i].width && Screen.height == Screen.resolutions[i].height) {
        //        ResolutionIndex = i;
        //    }
        //}
        //ResolutionDropdown.value = ResolutionIndex;
        //ResolutionDropdown.RefreshShownValue();
    }
	
	// Update is called once per frame
	void Update () {
        //Index.text = ResolutionIndex.ToString();
        if(Input.GetKeyUp(KeyCode.Tab) && (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift))) {
            if(ElementIndex - 1 < 0) {
                ElementIndex = Elements.Length - 1;
            } else {
                ElementIndex--;
            }
            Elements[ElementIndex].Select();
        } else if(Input.GetKeyUp(KeyCode.Tab)) {
            if (ElementIndex + 1 >= Elements.Length) {
                ElementIndex = 0;
            } else {
                ElementIndex++;
            }
            Elements[ElementIndex].Select();
        }
	}

    public void OnSFXChanged(float v) {
        if(sfx == null) {
            return;
        }
        sfx.volume = v;
        if(!sfx.isPlaying) {
            sfx.Play();
        }
        SoundManager.MaxSFXVolume = v;
    }

    public void OnBGMChanged(float v) {
        SoundManager.MaxBGMVolume = v;
    }

    //public void SetFullScreen(string on) {
    //    if(on == "true") {
    //        Fullscreen = true;
    //        FullscreenOFF.colors = Transparent;
    //        FullscreenON.colors = Solid;
    //    } else {
    //        Fullscreen = false;
    //        FullscreenOFF.colors = Solid;
    //        FullscreenON.colors = Transparent;
    //    }
    //}

    //public void ChangeResolution() {
    //    int index = ResolutionDropdown.value;
    //    Resolution r = Screen.resolutions[Screen.resolutions.Length - index - 1];
    //    Screen.SetResolution(r.width, r.height, Screen.fullScreen);
    //}

    public void OnSettingsOK() {
        //Screen.fullScreen = Fullscreen;
        PlayerPrefs.SetFloat("InitialSFXVolume", SoundManager.MaxSFXVolume);
        PlayerPrefs.SetFloat("InitialBGMVolume", SoundManager.MaxBGMVolume);
        SceneManager.LoadScene("MainMenu");
    }

    public void Exit() {
        SceneManager.LoadScene("MainMenu");
        SoundManager.MaxSFXVolume = InitialSFXVolume;
        SoundManager.MaxBGMVolume = InitialBGMVolume;
    }
}
