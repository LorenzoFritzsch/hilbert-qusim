# hilbert-qusim

**[Early Version]**

_hilbert-qusim_ is a state-vector simulator of quantum circuits. It considers to be working with logical qubits, in complete absence of noise.

---

# Introduction

_hilbert-qusim_, as a Schrödinger full-state vector simulator, simulates quantum computations by applying transformations to a state vector,
namely a vector of complex, single-precision, numbers. It does so by applying gates via matrix-vector multiplications. To do so efficiently,
operations are lazily built and materialised only at the end, using multi-threaded SIMD strategies (via AVX or Apple Accelerate instructions).

