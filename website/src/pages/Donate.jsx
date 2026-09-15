export default function Donate() {
  return (
    <main className="flex-1 bg-zinc-950 text-white px-4 sm:px-8 py-16">
      <section className="max-w-2xl mx-auto text-center">

        <h1 className="text-4xl sm:text-5xl font-bold tracking-tight">
          Support Maryn Launcher
        </h1>
        <p className="mt-6 text-zinc-400 text-lg">
          Maryn Launcher is free and open source. If you find it useful, consider supporting development.
        </p>

        <div className="mt-10 bg-zinc-900 border border-zinc-800 rounded-xl p-8">
          <p className="text-zinc-300 text-sm mb-6">
            Donations help cover development time and keep the project alive. Every contribution is appreciated.
          </p>
          <div className="inline-block bg-zinc-700 text-zinc-400 px-8 py-3 rounded-lg font-semibold cursor-not-allowed">
            Donations Coming Soon
          </div>
        </div>

        <p className="mt-8 text-zinc-500 text-sm">
          You can also star or contribute to the project on{" "}
          
            <a href="https://github.com/DinankaKoirala/Maryn_Launcher"
            target="_blank"
            rel="noopener noreferrer"
            className="text-zinc-300 hover:text-white underline transition-colors"
          >
            GitHub
          </a>
          .
        </p>

      </section>
    </main>
  )
}