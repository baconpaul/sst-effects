//
// Created by Paul Walker on 4/23/23.
//

#ifndef SURGE_CONCRETECONFIG_H
#define SURGE_CONCRETECONFIG_H

namespace sst::effects
{
/**
 * ConcreteConfig is a simple implementation of the Configuration protocol which
 * makes some simplifying assumptions (never temposync, use local tables, etc...)
 * but which allows you to implement a runnable version of the effects for simple
 * DSP tasks. It was *almost* just a regtest class but then I thought it might be
 * useful to people who "just want a flanger with 7 params" type thing.
 *
 * TODO: This is still a WIP so these need some more testing.
 */
struct ConcreteConfig
{
    struct BC
    {
        static constexpr uint16_t maxParamCount{20};
        float paramStorage[maxParamCount];
        template <typename... Types> BC(Types...) {}
    };

    struct GS
    {
        double sampleRate;

        // It is painful that sst-filters makes us over-adapt
        // this class
        GS(double sr) : sampleRate(sr) {}
    };
    struct ES
    {
    };

    using BaseClass = BC;
    using GlobalStorage = GS;
    using EffectStorage = ES;
    using ValueStorage = float *;
    using BiquadAdapter = ConcreteConfig;

    static constexpr int blockSize{16};

    static inline float floatValueAt(const BaseClass *const e, const ValueStorage *const v, int idx)
    {
        return e->paramStorage[idx];
    }
    static inline int intValueAt(const BaseClass *const e, const ValueStorage *const v, int idx)
    {
        return (int)std::round(e->paramStorage[idx]);
    }

    static inline float envelopeRateLinear(GlobalStorage *s, float f)
    {
        return 1.f * blockSize / (s->sampleRate) * pow(-2, f);
    }

    static inline float temposyncRatio(GlobalStorage *s, EffectStorage *e, int idx) { return 1; }

    static inline bool isDeactivated(EffectStorage *e, int idx) { return false; }

    static inline float rand01(GlobalStorage *s) { return (float)rand() / (float)RAND_MAX; }

    static inline double sampleRate(GlobalStorage *s) { return s->sampleRate; }
    static inline double sampleRateInv(GlobalStorage *s) { return 1.0 / s->sampleRate; }

    static inline float noteToPitch(GlobalStorage *s, float p) { return pow(2.0, p / 12); }
    static inline float noteToPitchIgnoringTuning(GlobalStorage *s, float p)
    {
        return noteToPitch(s, p);
    }

    static inline float noteToPitchInv(GlobalStorage *s, float p)
    {
        return 1.0 / noteToPitch(s, p);
    }

    static inline float dbToLinear(GlobalStorage *s, float f) { return 1; }
};
} // namespace sst::effects

#endif // SURGE_CONCRETECONFIG_H
