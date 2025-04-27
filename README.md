# AFNOR NF S32-002

> [!CAUTION]
> This project is provided for educational and lawful use only. Any use, deployment, or modification of this implementation must comply with all applicable local, national, and international laws and regulations, including those governing public infrastructure and accessibility devices. Unauthorized or non-legitimate use, especially in public spaces or without proper authorization from relevant authorities, is strictly prohibited. The authors and contributors are not responsible for any misuse or illegal application of this code.

Public Standard is available [here](https://norminfo.afnor.org/norme/nf-s32-002/dispositifs-repetiteurs-de-feux-de-circulation-a-lusage-des-personnes-aveugles-ou-malvoyantes/74403) ([buy](https://www.boutique.afnor.org/fr-fr/norme/nf-s32002/dispositifs-repetiteurs-de-feux-de-circulation-a-lusage-des-personnes-aveug/fa125183/650))

> [!NOTE]  
> Implementation of the French norm NF S32-002, which defines the functional and technical requirements for audible and tactile pedestrian signal repeaters at traffic lights for blind and visually impaired people.
>
> This includes synchronization with pedestrian signals, specific sound patterns and messages, volume ranges, and remote activation, ensuring accessibility and safety at crossings

## Dependencies

The only dependency is libhackrf, install it with your favorite package manager or from sources:

```
sudo emerge -a net-wireless/hackrf-tools
```

## Building & Running

Check hackrf is up and running:

``` bash
$ hackrf_info
hackrf_info version: 2023.01.1
libhackrf version: 2023.01.1 (0.8)
Found HackRF
Index: 0
Serial number: 00000xxxxxxxxxxxxxxxxxxxx
Board ID Number: 2 (HackRF One)
Firmware Version: 2023.01.1 (API:1.07)
Part ID Number: 0xa00xxxxx 0xxxxxxxx
Hardware Revision: older than r6
Hardware supported by installed firmware:
```

Then build and run:

```bash
$ make
$ ./afnor_nf_s32_002
OK
```

# French Law

- **Article 7 of the Decree of 23 September 2015**: This decree requires that audible pedestrian signals must emit a sound when the pedestrian light is green and an audio message starting with "red pedestrian" when the light is red. The message must also include the name of the street for all new or renovated intersections
- **NF S32-002 Standard**: This standard defines the technical and functional requirements for audible and tactile pedestrian signal repeaters, including their activation (manual button or remote control), sound patterns, and compliance with accessibility laws
- **2005 Disability Act**: The broader legislative context is provided by the 2005 Disability Act, which mandates the accessibility of public roads and infrastructure for people with disabilities, forming the legal foundation for the use of such devices
