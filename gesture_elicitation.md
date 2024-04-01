# Gesture elicitation

## Context

To increase the input bandwidth and ease of use of our system, we will add
gestures to the input modalities. In order to properly implement the gesture
recognition setup, such that it is inviting to use with high usability, we will
need to conduct a proper research a prior.

Let us first define the referents. The referents include two basic gestures
which aim to trigger the most common interactions with our watering system:

- Watering the plant
- Reading the sensor values

### Intended use of gestures

We would like to recognize the 'water plant' gesture, to trigger the plant
watering system such that it waters the plant with a reasonable amount - even if
the moist readings of the system indicate that the soil is saturated with enough
water.

The 'read sensors' gesture must trigger the system to not only read the sensors
at instant, but also share the resulting measurements through all possible
media: the OLED and the MQTT network. The previously scheduled sensor readings
could be rescheduled such that it does not measure all sensors within a short
timespan again.

### Target audience

The target audience of our plant watering system, and thus our gesture
elicitation, consists of tech-savvy users of any age who have an understanding
of how the sensors of our system might operate. We expect our target audience to
not make incorrect assumptions on if they should 'adapt' their usage of the
system in order to make the system understand them. This means that we assume
that the elicited gestures are unbiased.

#### Demographics of participants

|-----|-----|--------|--------------|-----------------------------|---------------------------|
| No. | Age | Gender | Right-handed | Commonly uses digital tools | Used gesture tools before |
|-----|-----|--------|--------------|-----------------------------|---------------------------|
| 1   | 45  | Female | Yes          | Yes                         | No                        |
| 2   | 49  | Male   | Yes          | Yes                         | No                        |

## Method

Each participant will be researched individually, isolated from other
participants. The researcher will have multiple symbols prepared, which can be
proposed to the participant, but won't be presented yet. All symbols in this
research are represented by simply showing the gesture with the body - no
animations or videos are used.

Firstly, the participant is asked to think of a possible symbol which fits the
referent, to their opinion. After agreeing upon that they would opt for that
symbol, the researcher presents the other symbols he prepared. The participant
is then asked to choose a symbol from all symbols discussed: those presented and
those the participant came up with. The participant should choose the symbol
which he/she/it believes is the symbol which best fits the referent. The symbol
the participant chooses now, is called the final symbol.

Lastly, the participant is asked to evaluate the final symbol on its complexity.

The results of the interaction with a single participant, include the original
symbol and the final symbol. The symbols should be described with an eye for
detail, to esure high reproducibility.

### Proposed symbols

These are the symbols which the researcher should have prepared in advance.

**Watering the plant**:

- Tilting a small watering can forwards
  - Like you hold a stick diagonally, leaning forwards, but start pointing it in
    front of you. The motion is smooth, and after holding it for a second or two
    tops, you return to your original pose, smoothly.
- Tilting a small watering can sideways
  - Same as the previous symbol, but you rotate everything by 90 degrees
    towards the other hand.
- Tilting a large watering can
  - Do the same as when you tilt the small watering can forwards, but act as if
    the stick is really big and you need a second hand.
- Sprinkling water droplets
  - Just like how you would spread salt onto your enemy's corpse in a video game,
    you slide your thumb along the other four fingers of the same hand, while
    moving said four fingers a little. Or you could compare it to applying cheese
    to your pizza.

**Reading sensor data**:

- Wave towards you
  - Wave towards you, as if you ask someone to approach you
- Native Amarican
  - Look forwards while putting your hand above your eyes to protect them from
    the sun
- Head scratch
  - Just get rid of that itch
- Sliding over the table
  - Move your hand downwards and slide it over an imaginary table. As if you are
    gesturing it's all over, but more slowly.

## Results

### All symbols

First, we will list all the final symbols observed:

**Watering the plant**:

|-----|--------------------------------------------|
| No. | Detailed description of final gesture      |
|-----|--------------------------------------------|
| 1   | Watering can sideways, but pssh if outside |
| 2   | Psssh                                      |

**Reading sensor data**:

|-----|---------------------------------------|
| No. | Detailed description of final gesture |
|-----|---------------------------------------|
| 1   | Wave, as in greeting                  |
| 2   | Wave towards you                      |

TODO LIST ALL FINAL SYMBOLS

### Dissimilarity matrix

To scientifically determine which gestures are to be considered similar, we will
apply a dissimilarity function on every pair of symbols, creating a
dissimilarity matrix.

The dissimilarity function we will use is to manually rate the dissimilarity on
a scale of 0 to 10. While this is no reproducible science, comparing things like
number of joints involved creates results which do not properly represent the
human understanding and associations of the gestures.

TODO HIER DISSIMILARITY MATRIX AFMAKEN

**Watering the plant**:

|---|----|-------------------------------------|
|   | 1  | 2                                   |
|---|----|-------------------------------------|
| 1 | 0  | 2                                   |
|---|----|-------------------------------------|
| 2 | 2  | 0                                   |
|---|----|-------------------------------------|

**Reading sensor data**:

|---|----|-------------------------------------|
|   | 1  | 2                                   |
|---|----|-------------------------------------|
| 1 | 0  | 4                                   |
|---|----|-------------------------------------|
| 2 | 4  | 0                                   |
|---|----|-------------------------------------|

### Clustering by dissimilarity

To express which symbols should be considered similar, we must compare all
dissimilarities to a τ, where τ represents the maximum dissimilarity before we
consider two symbols to be different. If the dissimilarity of two symbols is
lower than τ, then we consider them similar. We can express this in a matrix as
well. For this research, we consider τ to equal 4

TODO HIER EEN MATRIX MET BOOLEANS VOOR SIMILAR OR NOT SIMILAR


**Watering the plant**:

|---|----|-------------------------------------|
|   | 1  | 2                                   |
|---|----|-------------------------------------|
| 1 | T  | T                                   |
|---|----|-------------------------------------|
| 2 | T  | T                                   |
|---|----|-------------------------------------|

**Reading sensor data**:

|---|----|-------------------------------------|
|   | 1  | 2                                   |
|---|----|-------------------------------------|
| 1 | T  | F                                   |
|---|----|-------------------------------------|
| 2 | F  | T                                   |
|---|----|-------------------------------------|

This gives us the following clusters:

TODO HIER ALLE SYMBOLS GROEPEREN, ZODAT JE ZE ESSENTIEEL COMBINEERT TOT EEN PAAR
GESTURES DIE WE CONSDIREN

**Watering the plant**:

- 1: Psssh

**Reading sensor data**:

- 1: Wave, as in greeting
- 2: Wave towards you

We can treat each cluster as one gesture, so that we can compare these clusters
to determine which cluster is considered to be most fitting for each referent.

### Measure of agreement

To determine which gesture is considered the most fitting for each referent, we
must determine all agreement scores. The agreement score is calculated as
follows:

$$\sum{P_i \in P}{(|P_i|/|P|)^2}$$

Thus, this gives us the following measures of agreement for both referents:

TODO GEEF HIER PER CLUSTER AAN WAT DE MEASURE OF AGREEMNT IS

## Conclusion

TODO CHOOSE GESTURE WITH THE HIGHEST AGREEMENT SCORE.

